
#include "Inventory/PlayerEquipComponent.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/ItemData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Interaction/PlayerInteractComponent.h"
#include "Inventory/InventoryInfo.h"
#include "Inventory/ItemUsableComponent.h"
#include "Inventory/PickupableComponent.h"

void UPlayerEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	/** Add spring arm component */
	ItemAttachSpring = Cast<USpringArmComponent>(ItemAttachParent.GetComponent(GetOwner()));
	if (ItemAttachSpring)
		InitialSpringArmOffset = ItemAttachSpring->TargetOffset.Z;

	/** Find inventory component */
	InventoryCompRef = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (InventoryCompRef)
	{
		InventoryCompRef->OnInventoryChange.AddDynamic(this, &UPlayerEquipComponent::UpdateEquip);
		InventoryCompRef->GetInventory(OUT Inventory);
	}
	auto* InteractComp = GetOwner()->FindComponentByClass<UPlayerInteractComponent>();
	if (InteractComp)
	{
		InteractComp->OnInteract.AddDynamic(this, &UPlayerEquipComponent::ItemInteract);
	}

	OriginalSocketOffset = ItemAttachSpring->SocketOffset;
}

UPlayerEquipComponent::UPlayerEquipComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

// Called every frame
void UPlayerEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/** Interp equipped item to move smoothly into view */
	if (!GetEquippedItemData().ItemHandle.IsValid() && ItemAttachSpring)
	{
		const float CurrentOffset = ItemAttachSpring->TargetOffset.Z;
		if (!FMath::IsNearlyEqual(CurrentOffset, InitialSpringArmOffset))
			ItemAttachSpring->TargetOffset.Z = UKismetMathLibrary::FInterpTo(CurrentOffset, InitialSpringArmOffset, DeltaTime, EquipInterpSpeed);
	}
}

void UPlayerEquipComponent::EquipItem(const FInventoryContents& Item)
{
	if (!ensure(InventoryCompRef->ItemBaseClass)) { return; }
	UnequipItem();
	
	// Update custom class if needed
	TSubclassOf<AActor> ItemBaseClass = InventoryCompRef->ItemBaseClass;
	if (auto* ItemInfo = Item.ItemInformation)
	{
		ItemBaseClass = ItemInfo->bCustomClass ? ItemInfo->CustomClass : ItemBaseClass;
	}
	
	/** Spawn item and attach it to the player */
	AActor* Pickupable = GetWorld()->SpawnActor<AActor>(ItemBaseClass, GetOwner()->GetTransform());
	FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	
	Pickupable->AttachToComponent(Cast<USceneComponent>(ItemAttachParent.GetComponent(GetOwner())), TransformRules);
	Pickupable->SetActorEnableCollision(false);

	EquippedItem = Item;
	EquippedActor = Pickupable;
	
	if (auto* Mesh = Pickupable->FindComponentByClass<UStaticMeshComponent>())
	{
		Mesh->SetSimulatePhysics(false);
		if (!GetEquippedItemData().ItemHandle.IsValid() && !GetEquippedItemData().ItemInformation->bCustomClass)
		{
			Mesh->SetStaticMesh(GetEquippedItemInfo()->ItemMesh.Get());
		}
	}

	if (UPickupableComponent* PickupComp = Pickupable->FindComponentByClass<UPickupableComponent>())
	{
		PickupComp->ItemData = Item;
		PickupComp->bPlayerInteract = false;
	}
	
	if (ItemAttachSpring)
	{
		ItemAttachSpring->SocketOffset = OriginalSocketOffset;
		
		if (auto* ItemInfo = Item.ItemInformation)
		{
			ItemAttachSpring->SocketOffset += ItemInfo->ItemOffset;
		}
		
		ItemAttachSpring->TargetOffset.Z = ItemUnequipOffset;
	}
		
}

void UPlayerEquipComponent::UnequipItem()
{
	UActorComponent* AttachedComp = ItemAttachParent.GetComponent(GetOwner());
	if (!AttachedComp) { return; }
	
	USceneComponent* ItemAttach = Cast<USceneComponent>(AttachedComp);
	if (ensureMsgf(ItemAttach, TEXT("ItemAttach component is not a scene component!")))
	{
		/** Unequip any items that were binded to the actor */
		TArray<AActor*> ItemsAttached;
		GetOwner()->GetAttachedActors(OUT ItemsAttached);
		for (AActor* Item : ItemsAttached)
		{
			Item->Destroy();
		}
		EquippedItem = FInventoryContents();
		if (ItemAttachSpring)
			ItemAttachSpring->TargetOffset.Z = InitialSpringArmOffset;
	}
}

FInventoryContents UPlayerEquipComponent::GetEquippedItemData() const
{
	return EquippedItem;
}

UItemInformation* UPlayerEquipComponent::GetEquippedItemInfo() const
{
	if (UItemInformation* ItemInfo = EquippedItem.ItemInformation)
	{
		return ItemInfo;
	}
	return nullptr;
	
}

AActor* UPlayerEquipComponent::GetEquippedItem() const
{
	return EquippedActor;
}

void UPlayerEquipComponent::DropEquippedItem()
{
	if (!GetEquippedItemData().ItemHandle.IsValid())
	{
		/** Unequip any items that were binded to the actor */
		TArray<AActor*> ItemsAttached;
		GetOwner()->GetAttachedActors(OUT ItemsAttached);
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetOwner<APawn>()->GetControlRotation());
		for (AActor* Item : ItemsAttached)
		{
			Item->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
			Item->SetActorEnableCollision(true);

			/** Re-enable physics and add throwing impulse */
			UStaticMeshComponent* Mesh = Item->FindComponentByClass<UStaticMeshComponent>();
			if (Mesh)
			{
				Mesh->SetSimulatePhysics(true);
				Mesh->AddImpulse(ForwardVector * ThrowImpulse);
			}

			if (UPickupableComponent* Pickupable = Item->FindComponentByClass<UPickupableComponent>())
			{
				Pickupable->bPlayerInteract = true;
			}
		}
		
		/** Remove item from inventory */
		InventoryCompRef->RemoveFromInventory(GetEquippedItemData().ItemHandle);

		EquippedItem = FInventoryContents();
		EquippedActor = nullptr;
	}
}

void UPlayerEquipComponent::UpdateEquip(const TArray<FInventoryContents>& NewInventory)
{
	InventoryCompRef->GetInventory(OUT Inventory);
	if (true)
	{
		/** Equip the item that was just picked up if the player is empty handed */
		if (EquippedItem.ItemHandle.IsValid() && Inventory.Num() > 0)
			EquipItem(Inventory[Inventory.Num()-1]);
	}
	else
	{
		/** Ensure that equipped item still exists */
		//if (InventoryCompRef->FindItemSlot(GetEquippedItemData()) == -1)
			UnequipItem();
	}
}

void UPlayerEquipComponent::ItemInteract(UInteractableComponent* Interactable)
{
	if (GetEquippedItemData().ItemHandle.IsValid()) { return; }
	
	if (Interactable)
	{
		/** Attempt to use item on viewed object */
		if (UItemUsableComponent* ItemUsableComponent = Interactable->GetOwner()->FindComponentByClass<UItemUsableComponent>())
		{
			ItemUsableComponent->OnItemUse.Broadcast(GetOwner(), GetEquippedItemData());
		}
	}
	else
	{
		/** Attempt to use the item in hand */
		OnItemUse.Broadcast(GetEquippedItemData());
	}
}
 