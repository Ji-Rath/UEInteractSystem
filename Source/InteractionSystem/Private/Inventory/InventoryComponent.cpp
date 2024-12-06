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

bool UInventoryComponent::AddToInventory(const FInventoryContents& Item, FItemHandle& OutItemHandle)
{
	if (!ensureMsgf(Item.IsValid(), TEXT("Invalid item to add to inventory"))) { return false; }
	
	auto CountToAdd = Item.Count;
	
	for (auto& InventoryContents : Inventory.Items)
	{
		// If the item is already in the inventory
		if (InventoryContents.ItemInformation == Item.ItemInformation)
		{
			int NewStack = InventoryContents.Count + CountToAdd;
			if (InventoryContents.ItemInformation->CanStack(NewStack))
			{
				UE_LOG(LogInventory, Log, TEXT("Stacked item %s in inventory"), *Item.ItemInformation->DisplayName.ToString());
				CountToAdd = InventoryContents.AddToStack(Item.Count);
				Inventory.MarkItemDirty(InventoryContents);
				OnItemChange.Broadcast(InventoryContents);
				OnRep_Inventory();
				
				if (CountToAdd == 0)
				{
					OutItemHandle = InventoryContents.ItemHandle;
					return true;
				}
			}
		}
	}
	
	// If the item is not in the inventory
	if (CanAddToInventory(Item))
	{
		UE_LOG(LogInventory, Log, TEXT("Added item %s to inventory"), *Item.ItemInformation->DisplayName.ToString());
		
		auto ItemCopy = Item;
		ItemCopy.OwnerComp = this;
		
		auto& ItemAdded = Inventory.Items.Emplace_GetRef(ItemCopy);
		OutItemHandle = ItemAdded.ItemHandle;
		OnRep_Inventory();
		OnItemAdd.Broadcast(ItemAdded);
		Inventory.MarkItemDirty(ItemAdded);
		return true;
	}
	else
	{
		UE_LOG(LogInventory, Warning, TEXT("Failed to add item %s to inventory"), *Item.ItemInformation->DisplayName.ToString());
	}
	
	return false;
}

void UInventoryComponent::EmptyInventory()
{
	Inventory.Items.Empty();
	Inventory.MarkArrayDirty();
	OnRep_Inventory();
}

bool UInventoryComponent::CanAddToInventory(const FInventoryContents& Item) const
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

FInventoryContents UInventoryComponent::GenerateItem(UItemInformation* ItemInfo, const FInstancedStruct& DynamicData,
                                                     int Count) const
{
	auto NewItem = FInventoryContents(ItemInfo, Count, DynamicData);
	NewItem.ItemHandle = GenerateUniqueHandle();
	
	return NewItem;
}

void UInventoryComponent::OnRep_Inventory()
{
	OnInventoryChange.Broadcast(Inventory.Items);
}

