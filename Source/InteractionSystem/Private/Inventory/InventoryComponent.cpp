// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Inventory/InventoryInfo.h"
#include "Interaction/ItemData.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogInventory);

static uint64 IDNum = 0;

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	// ...
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Inventory);
}

void UInventoryComponent::RemoveFromInventory(const FItemHandle& ItemHandle)
{
	int Index = Inventory.Items.IndexOfByKey(ItemHandle);
	if (Index != INDEX_NONE)
	{
		Inventory.Items.RemoveAtSwap(Index);
		OnRep_Inventory();
		Inventory.MarkArrayDirty();
	}
}

FInventoryContents UInventoryComponent::GetItemByHandle(const FItemHandle& ItemHandle) const
{
	if (auto ItemContents = Inventory.Items.FindByKey(ItemHandle))
	{
		return *ItemContents;
	}
	
	return FInventoryContents();
}

FItemHandle UInventoryComponent::FindItemByData(const UItemInformation* ItemData) const
{
	if (auto Item = Inventory.Items.FindByKey(ItemData))
	{
		return Item->ItemHandle;
	}
	
	return FItemHandle();
}

bool UInventoryComponent::AddToInventory(const FItemData& Item, FItemHandle& OutItemHandle)
{
	if (!ensureMsgf(Item.IsValid(), TEXT("Invalid item to add to inventory"))) { return false; }
	
	auto CountToAdd = Item.Count;

	while (CountToAdd > 0)
	{
		// First try finding existing stack (that has room)
		FInventoryContents* ItemInInventory = nullptr;
		for (auto& InventoryContents : Inventory.Items)
		{
			if (InventoryContents == Item && Item.HasRoom())
			{
				ItemInInventory = &InventoryContents;
				break;
			}
		}
		
		if (ItemInInventory)
		{
			UE_LOG(LogInventory, Log, TEXT("Stacked item %s in inventory"), *Item.ItemInformation->DisplayName.ToString());
			CountToAdd = ItemInInventory->AddToStack(Item.Count);
			Inventory.MarkItemDirty(*ItemInInventory);
			OnItemChange.Broadcast(*ItemInInventory);
			OnRep_Inventory();
			OutItemHandle = ItemInInventory->ItemHandle;
		}
		else
		{
			// Else, add to inventory
			if (CanAddToInventory(Item))
			{
				UE_LOG(LogInventory, Log, TEXT("Added item %s to inventory"), *Item.ItemInformation->DisplayName.ToString());
		
				auto ItemCopy = Item;
				ItemCopy.Count = CountToAdd;
				CountToAdd = ItemCopy.FixCount();
		
				auto& ItemAdded = Inventory.Items.Emplace_GetRef(GenerateItem(TInstancedStruct<FItemData>::Make(ItemCopy)));
				OutItemHandle = ItemAdded.ItemHandle;
				OnRep_Inventory();
				OnItemAdd.Broadcast(ItemAdded);
				Inventory.MarkItemDirty(ItemAdded);
			}
			else
			{
				// We return false here as there is no other option
				UE_LOG(LogInventory, Warning, TEXT("Failed to add item %s to inventory"), *Item.ItemInformation->DisplayName.ToString());
				return false;
			}
		}
	}
	return true;
}

void UInventoryComponent::EmptyInventory()
{
	Inventory.Items.Empty();
	Inventory.MarkArrayDirty();
	OnRep_Inventory();
}

bool UInventoryComponent::CanAddToInventory(const FItemData& Item) const
{
	return true;
}

void UInventoryComponent::GetInventory(TArray<FInventoryContents>& OutInventory) const
{
	OutInventory = Inventory.Items;
}

void UInventoryComponent::SetInventory(const TArray<FInventoryContents>& NewInventory)
{
	Inventory.Items = NewInventory;
	Inventory.MarkArrayDirty();
	OnRep_Inventory();
}

FItemHandle UInventoryComponent::GenerateUniqueHandle()
{
	return FItemHandle(IDNum++);
}

FInventoryContents UInventoryComponent::FindItemByHandle(const FItemHandle& ItemHandle)
{
	for(TObjectIterator<UInventoryComponent> Itr; Itr; ++Itr)
	{
		// Skip template object
		if (Itr->IsTemplate()) { continue; }
		
		auto Item = Itr->GetItemByHandle(ItemHandle);
		if (Item.IsValid()) { return Item; }
	}
	
	return FInventoryContents();
}

FInventoryContents UInventoryComponent::GenerateItem(const TInstancedStruct<FItemData>& ItemData)
{
	auto NewItem = FInventoryContents(GenerateUniqueHandle(), ItemData, this);
	
	return NewItem;
}

void UInventoryComponent::OnRep_Inventory()
{
	OnInventoryChange.Broadcast(Inventory.Items);
}

void UInventoryComponent::RemoveItemFromInventory(const FItemData& Item)
{
	int Amount = Item.Count;
	auto InventoryItem = Inventory.Items.FindByKey(Item);
	while (InventoryItem && Amount > 0)
	{
		UE_LOG(LogInventory, Log, TEXT("Want to remove: %d - Item has %d"),Amount, InventoryItem->Item.Get().Count);
		Amount = InventoryItem->RemoveFromStack(Amount);
		if (InventoryItem->Item.Get().Count <= 0)
		{
			RemoveFromInventory(InventoryItem->ItemHandle);
		}
		InventoryItem = Inventory.Items.FindByKey(Item);
		UE_LOG(LogInventory, Log, TEXT("Amount = %d"), Amount)
	}
}
