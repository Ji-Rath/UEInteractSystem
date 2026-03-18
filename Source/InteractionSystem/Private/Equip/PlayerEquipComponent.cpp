
#include "Equip/PlayerEquipComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryInfo.h"
#include "Inventory/InventoryLibrary.h"
#include "Inventory/InventorySettings.h"
#include "Inventory/Pickupable.h"
#include "Item/ItemData.h"
#include "Kismet/GameplayStatics.h"

DECLARE_LOG_CATEGORY_CLASS(LogPlayerEquip, Log, All);

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
	Super::EquipItem_Implementation(Item);
	
	if (!Item.IsValid())
	{
		UE_LOG(LogPlayerEquip, Error, TEXT("%s: EquipItem_Implementation: Item is invalid"), *GetOwner()->GetName());
		return;
	}
	
	UnequipItem();

	const FInventoryContents& ItemContents = InventoryComponent->GetItemDataChecked(Item);
	if (!ItemContents.IsValid())
	{
		UE_LOG(LogPlayerEquip, Error, TEXT("%s: Item does not exist!"), *GetOwner()->GetName());
		return;
	}
	
	FItemActorSpawned OnActorSpawned;
	OnActorSpawned.BindDynamic(this, &ThisClass::EquipActorSpawned);
	UInventoryLibrary::SpawnItem(this, ItemContents.ItemData.Get().ItemInformation);
}

void UPlayerEquipComponent::UnequipItem_Implementation()
{
	Super::UnequipItem_Implementation();
	
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
		
		if (ItemAttachSpring)
			ItemAttachSpring->TargetOffset.Z = InitialSpringArmOffset;
	}
}

AActor* UPlayerEquipComponent::GetEquippedActor() const
{
	return EquippedActor;
}

void UPlayerEquipComponent::EquipActorSpawned(AItemVisual* Actor)
{
	/** Spawn item and attach it to the player */
	
	FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
	
	Actor->AttachToComponent(Cast<USceneComponent>(ItemAttachParent.GetComponent(GetOwner())), TransformRules);
	Actor->SetActorEnableCollision(false);
	
	EquippedActor = Actor;
	
	if (ItemAttachSpring)
	{
		ItemAttachSpring->SocketOffset = OriginalSocketOffset;
		ItemAttachSpring->TargetOffset.Z = ItemUnequipOffset;
	}
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
 