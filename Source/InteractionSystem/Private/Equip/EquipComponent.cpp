// Fill out your copyright notice in the Description page of Project Settings.


#include "Equip/EquipComponent.h"

#include "Engine/AssetManager.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryLibrary.h"
#include "Item/ItemAction.h"
#include "Item/ItemData.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UEquipComponent::UEquipComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	/** Find inventory component */
	InventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryChange.AddDynamic(this, &ThisClass::UpdateEquip);
		InventoryComponent->OnItemAdd.AddDynamic(this, &ThisClass::ItemAdded);
	}
}

void UEquipComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryChange.RemoveDynamic(this, &ThisClass::UpdateEquip);
		InventoryComponent->OnItemAdd.RemoveDynamic(this, &ThisClass::ItemAdded);
	}
	
	if (HasItemEquipped())
	{
		UnequipItem();
	}
}

void UEquipComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipComponent, EquippedItem);
}

void UEquipComponent::ItemAdded(const FInventoryContents& Item)
{
	if (bAutoEquip && !HasItemEquipped())
	{
		EquipItem(Item.ItemHandle);
	}
}

bool UEquipComponent::HasItemEquipped() const
{
	return EquippedItem.IsValid();
}

bool UEquipComponent::IsUsingItem() const
{
	return bUsingItem;
}

FItemHandle UEquipComponent::GetEquippedItem() const
{
	return EquippedItem;
}

UItemInformation* UEquipComponent::GetEquippedItemInfo() const
{
	const FInventoryContents& Item = InventoryComponent->GetItemDataChecked(EquippedItem);
	if (!Item.IsValid()) { return nullptr; }
	
	if (UItemInformation* ItemInfo = Item.ItemData.Get().ItemInformation)
	{
		return ItemInfo;
	}
	
	checkNoEntry()
	return nullptr;
}

void UEquipComponent::UpdateEquip(const TArray<FInventoryContents>& NewInventory)
{
	/** Ensure that equipped item still exists */
	if (HasItemEquipped() && !InventoryComponent->GetItemDataChecked(GetEquippedItem()).IsValid())
		UnequipItem();
}

void UEquipComponent::UseItem()
{
	if (!EquippedItem.IsValid()) { return; }

	const FInventoryContents& Item = UInventoryComponent::FindItemByHandle(EquippedItem);
	UItemInformation* ItemInfo = Item.GetItemInformation();
	if (!ItemInfo) { return; }

	CurrentAction = DuplicateObject(ItemInfo->Action, GetOwner()); 
	if (!CurrentAction) { return; }
	
	bUsingItem = true;
	CurrentAction->ExecuteAction(EquippedItem, GetOwner());
	OnUpdateUseState.Broadcast(EquippedItem, true);
}

void UEquipComponent::FinishUseItem()
{
	if (!EquippedItem.IsValid()) { return; }
	if (!ensure(CurrentAction)) { return; } // We can only finish an action if we started it previously

	const FInventoryContents& Item = UInventoryComponent::FindItemByHandle(EquippedItem);
	UItemInformation* ItemInfo = Item.GetItemInformation();
	if (!ItemInfo) { return; }
	
	bUsingItem = false;
	CurrentAction->FinishExecuteAction(EquippedItem, GetOwner());
	OnUpdateUseState.Broadcast(EquippedItem, false);
	CurrentAction = nullptr;
}

void UEquipComponent::UnequipItem_Implementation()
{
	if (!EquippedItem.IsValid()) { return; }
	
	UAssetManager& Manager = UAssetManager::Get();
	Manager.ChangeBundleStateForPrimaryAssets({GetEquippedItemInfo()->GetPrimaryAssetId()}, {}, {"Visual"});
	
	EquippedItem = FItemHandle();
	OnUpdateEquipState.Broadcast(FItemHandle());
}

void UEquipComponent::ToggleEquippedItem(const FItemHandle& Item)
{
	if (EquippedItem == Item)
	{
		UnequipItem();
	}
	else
	{
		EquipItem(Item);
	}
}

void UEquipComponent::EquipItem_Implementation(const FItemHandle& Item)
{
	if (!Item.IsValid()) { return; }
	if (Item == EquippedItem) { return; }
	
	EquippedItem = Item;
	
	check(GetEquippedItemInfo());
	
	UAssetManager& Manager = UAssetManager::Get();
	Manager.LoadPrimaryAsset(GetEquippedItemInfo()->GetPrimaryAssetId(), {"Visual"}, 
		FStreamableDelegate::CreateWeakLambda(this, [&, EquipItem = Item]()
		{
			OnUpdateEquipState.Broadcast(EquipItem);
		}));
}

