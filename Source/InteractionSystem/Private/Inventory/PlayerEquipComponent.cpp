
#include "Inventory/PlayerEquipComponent.h"

#include "Pickupable.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/InteractorComponent.h"
#include "Interaction/ItemAction.h"
#include "Interaction/ItemData.h"
#include "Interaction/Usable.h"
#include "Kismet/KismetMathLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryInfo.h"
#include "Inventory/InventorySettings.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

DECLARE_LOG_CATEGORY_CLASS(LogPlayerEquip, Log, All);

void UPlayerEquipComponent::ItemAdded(const FInventoryContents& Item)
{
	if (!HasItemEquipped())
	{
		EquipItem(Item.ItemHandle);
	}
}

void UPlayerEquipComponent::BeginPlay()
{
	Super::BeginPlay();
	
	/** Add spring arm component */
	ItemAttachSpring = Cast<USpringArmComponent>(ItemAttachParent.GetComponent(GetOwner()));
	if (ItemAttachSpring)
	{
		OriginalSocketOffset = ItemAttachSpring->SocketOffset;
		InitialSpringArmOffset = ItemAttachSpring->TargetOffset.Z;
	}

	/** Find inventory component */
	InventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryChange.AddDynamic(this, &UPlayerEquipComponent::UpdateEquip);
		InventoryComponent->OnItemAdd.AddDynamic(this, &ThisClass::ItemAdded);
	}
}

void UPlayerEquipComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerEquipComponent, EquippedItem);
}

UPlayerEquipComponent::UPlayerEquipComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

// Called every frame
void UPlayerEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/** Interp equipped item to move smoothly into view */
	if (!GetEquippedItem().IsValid() && ItemAttachSpring)
	{
		const float CurrentOffset = ItemAttachSpring->TargetOffset.Z;
		if (!FMath::IsNearlyEqual(CurrentOffset, InitialSpringArmOffset))
			ItemAttachSpring->TargetOffset.Z = UKismetMathLibrary::FInterpTo(CurrentOffset, InitialSpringArmOffset, DeltaTime, EquipInterpSpeed);
	}
}

void UPlayerEquipComponent::EquipItem_Implementation(const FItemHandle& Item)
{
	if (!Item.IsValid())
	{
		UE_LOG(LogPlayerEquip, Error, TEXT("%s: EquipItem_Implementation: Item is invalid"), *GetOwner()->GetName());
		return;
	}
	
	UnequipItem();
	
	// Update custom class if needed
	TSubclassOf<AActor> ItemBaseClass = AStaticMeshActor::StaticClass();
	auto ItemContents = InventoryComponent->GetItemByHandle(Item);
	if (!ItemContents.IsValid())
	{
		UE_LOG(LogPlayerEquip, Error, TEXT("%s: Item does not exist!"), *GetOwner()->GetName());
	}
	if (auto* ItemInfo = ItemContents.Item.Get().ItemInformation)
	{
		ItemBaseClass = ItemInfo->bCustomClass ? ItemInfo->CustomClass : ItemBaseClass;
	}
	
	/** Spawn item and attach it to the player */
	AActor* Pickupable = GetWorld()->SpawnActor<AActor>(ItemBaseClass, ItemContents.Item.Get().ItemInformation->ItemOffset);
	
	if (auto SMA = Cast<AStaticMeshActor>(Pickupable))
	{
		SMA->SetMobility(EComponentMobility::Movable);
	}
	
	FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
	
	Pickupable->AttachToComponent(Cast<USceneComponent>(ItemAttachParent.GetComponent(GetOwner())), TransformRules);
	Pickupable->SetActorEnableCollision(false);

	EquippedItem = Item;
	EquippedActor = Pickupable;
	
	if (auto* Mesh = Pickupable->FindComponentByClass<UStaticMeshComponent>())
	{
		Mesh->SetSimulatePhysics(false);
		if (GetEquippedItem().IsValid() && !GetEquippedItemInfo()->bCustomClass)
		{
			Mesh->SetStaticMesh(GetEquippedItemInfo()->ItemMesh.Get());
		}
	}
	
	if (ItemAttachSpring)
	{
		ItemAttachSpring->SocketOffset = OriginalSocketOffset;

		auto ItemInformation = InventoryComponent->GetItemByHandle(Item);
		
		ItemAttachSpring->TargetOffset.Z = ItemUnequipOffset;
	}
		
}

bool UPlayerEquipComponent::HasItemEquipped() const
{
	return EquippedItem.IsValid();
}

void UPlayerEquipComponent::UnequipItem_Implementation()
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
		EquippedItem = FItemHandle();
		if (ItemAttachSpring)
			ItemAttachSpring->TargetOffset.Z = InitialSpringArmOffset;
	}
}

FItemHandle UPlayerEquipComponent::GetEquippedItem() const
{
	return EquippedItem;
}

UItemInformation* UPlayerEquipComponent::GetEquippedItemInfo() const
{
	auto Item = InventoryComponent->GetItemByHandle(EquippedItem);
	if (UItemInformation* ItemInfo = Item.Item.Get().ItemInformation)
	{
		return ItemInfo;
	}
	return nullptr;
	
}

AActor* UPlayerEquipComponent::GetEquippedActor() const
{
	return EquippedActor;
}

void UPlayerEquipComponent::DropEquippedItem()
{
	if (HasItemEquipped())
	{
		/** Unequip any items that were binded to the actor */
		TArray<AActor*> ItemsAttached;
		GetOwner()->GetAttachedActors(OUT ItemsAttached);
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetOwner<APawn>()->GetControlRotation());
		for (AActor* Item : ItemsAttached)
		{
			Item->Destroy();
		}

		PerformDropEquippedItem();
		
		if (!GetOwner()->HasAuthority())
		{
			ServerDropEquippedItem();
		}
	}
}

void UPlayerEquipComponent::PerformDropEquippedItem()
{
	auto InventorySettings = GetDefault<UInventorySettings>();
	check(InventorySettings);
	check(!InventorySettings->DefaultPickupable.IsNull());

	FTransform PickupableTransform = GetOwner()->GetTransform();
	PickupableTransform.SetLocation(PickupableTransform.GetLocation() + GetOwner()->GetActorForwardVector() * 100.f);
	auto Pickupable = GetWorld()->SpawnActorDeferred<APickupable>(InventorySettings->DefaultPickupable.LoadSynchronous(), PickupableTransform);
	Pickupable->Item = GetEquippedItemInfo();
	Pickupable->ProjectileMovement->Velocity = Pickupable->GetActorForwardVector() * ThrowImpulse;

	UGameplayStatics::FinishSpawningActor(Pickupable, PickupableTransform);

	/** Remove item from inventory */
	InventoryComponent->RemoveFromInventory(GetEquippedItem());
	
	EquippedItem = FItemHandle();
	EquippedActor = nullptr;
}

void UPlayerEquipComponent::ServerDropEquippedItem_Implementation()
{
	PerformDropEquippedItem();
}

void UPlayerEquipComponent::UpdateEquip(const TArray<FInventoryContents>& NewInventory)
{
	/** Ensure that equipped item still exists */
	if (!InventoryComponent->GetItemByHandle(GetEquippedItem()).IsValid())
		UnequipItem();
}

void UPlayerEquipComponent::UseItem()
{
	if (!EquippedItem.IsValid()) { return; }

	auto Item = UInventoryComponent::FindItemByHandle(EquippedItem);
	auto ItemInfo = Item.GetItemInformation<UUsable>();
	if (!ItemInfo) { return; }
	
	auto Action = ItemInfo->ActionConfig.GetPtr<FUsableAction>();
	if (!Action) { return; }
	
	Action->ExecuteAction(EquippedItem, GetOwner());
}

void UPlayerEquipComponent::FinishUseItem()
{
	if (!EquippedItem.IsValid()) { return; }

	auto Item = UInventoryComponent::FindItemByHandle(EquippedItem);
	auto ItemInfo = Item.GetItemInformation<UUsable>();
	if (!ItemInfo) { return; }
	
	auto Action = ItemInfo->ActionConfig.GetPtr<FUsableAction>();
	if (!Action) { return; }
	
	Action->FinishExecuteAction(EquippedItem, GetOwner());
}
 