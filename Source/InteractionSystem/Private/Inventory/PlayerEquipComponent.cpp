
#include "Inventory/PlayerEquipComponent.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Interaction/PlayerInteractComponent.h"
#include "Inventory/ItemDataComponent.h"
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
	if (EquippedItem && ItemAttachSpring)
	{
		const float CurrentOffset = ItemAttachSpring->TargetOffset.Z;
		if (!FMath::IsNearlyEqual(CurrentOffset, InitialSpringArmOffset))
			ItemAttachSpring->TargetOffset.Z = UKismetMathLibrary::FInterpTo(CurrentOffset, InitialSpringArmOffset, DeltaTime, EquipInterpSpeed);
	}
}

void UPlayerEquipComponent::EquipItem(UItemData* Item)
{
	UnequipItem();

	/** Spawn item and attach it to the player */
	if (Item)
	{
		TSubclassOf<AActor> ItemClass = Item->ActorClass;
		if (ensureMsgf(ItemClass, TEXT("%s ItemData does not contain valid class!"), *Item->GetName()))
		{
			AActor* Pickupable = GetWorld()->SpawnActor<AActor>(ItemClass, GetOwner()->GetTransform());
			FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

			Pickupable->AttachToComponent(Cast<USceneComponent>(ItemAttachParent.GetComponent(GetOwner())), TransformRules);
			Pickupable->SetActorEnableCollision(false);
			if (auto* Mesh = Pickupable->FindComponentByClass<UStaticMeshComponent>())
				Mesh->SetSimulatePhysics(false);
			EquippedItem = Pickupable;
			if (ItemAttachSpring)
				ItemAttachSpring->TargetOffset.Z = ItemUnequipOffset;
		}
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
		EquippedItem = nullptr;
		if (ItemAttachSpring)
			ItemAttachSpring->TargetOffset.Z = InitialSpringArmOffset;
	}
}

UItemData* UPlayerEquipComponent::GetEquippedItemData() const
{
	if (EquippedItem)
	{
		UItemDataComponent* DataComponent = EquippedItem->FindComponentByClass<UItemDataComponent>();
		if (DataComponent)
			return DataComponent->GetItemData();
	}
	
	return nullptr;
}

AActor* UPlayerEquipComponent::GetEquippedItem() const
{
	return EquippedItem;
}

void UPlayerEquipComponent::DropEquippedItem()
{
	if (EquippedItem)
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
		}
		/** Remove item from inventory */
		IInventoryInterface::Execute_RemoveFromInventory(InventoryCompRef, GetEquippedItemData(), 1);
	}
}

void UPlayerEquipComponent::UpdateEquip(bool bAdded)
{
	InventoryCompRef->GetInventory(OUT Inventory);
	if (bAdded)
	{
		/** Equip the item that was just picked up if the player is empty handed */
		if (!EquippedItem)
			EquipItem(Inventory[Inventory.Num()-1].ItemData);
	}
	else
	{
		/** Ensure that equipped item still exists */
		if (InventoryCompRef->FindItemSlot(GetEquippedItemData()) == -1)
			UnequipItem();
	}
}

void UPlayerEquipComponent::ItemInteract(UInteractableComponent* Interactable)
{
	if (!GetEquippedItem()) { return; }
	
	UPickupableComponent* Item = GetEquippedItem()->FindComponentByClass<UPickupableComponent>();
	UItemDataComponent* ItemDataComponent = GetEquippedItem()->FindComponentByClass<UItemDataComponent>();
	if (Item)
	{
		UItemData* ItemData = GetEquippedItemData();
		if (ItemData)
		{
			if (Interactable)
			{
				/** Attempt to use item on viewed object */
				if (UItemUsableComponent* ItemUsableComponent = Interactable->GetOwner()->FindComponentByClass<UItemUsableComponent>())
				{
					ItemUsableComponent->OnItemUse.Broadcast(GetOwner(), ItemData);
				}
			}
			else
			{
				/** Attempt to use the item in hand */
				Item->OnUsePickupable.Broadcast(GetOwner());
				OnItemUse.Broadcast(ItemDataComponent->GetItemData());
			}
		}
	}
}
 