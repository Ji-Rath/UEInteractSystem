// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Inventory/InventoryInfo.h"
#include "Interaction/ItemData.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogInventory);

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

void UInventoryComponent::DropItem(const FItemHandle& Item)
{
}

void UInventoryComponent::RemoveFromInventory(const FItemHandle& ItemHandle)
{
	if (int Index = Inventory.IndexOfByKey(ItemHandle))
	{
		Inventory.RemoveAtSwap(Index);
	}
}

FInventoryContents UInventoryComponent::GetItemByHandle(const FItemHandle& ItemHandle) const
{
	if (auto ItemContents = Inventory.FindByKey(ItemHandle))
	{
		return *ItemContents;
	}
	
	return FInventoryContents();
}

FItemHandle UInventoryComponent::FindItemByData(const UItemInformation* ItemData) const
{
	if (auto Item = Inventory.FindByKey(ItemData))
	{
		return Item->ItemHandle;
	}
	
	return FItemHandle();
}

bool UInventoryComponent::AddToInventory(const FInventoryContents& Item, FItemHandle& OutItemHandle)
{
	auto CountToAdd = Item.Count;
	
	for (auto& InventoryContents : Inventory)
	{
		// If the item is already in the inventory
		if (InventoryContents.ItemInformation == Item.ItemInformation)
		{
			int NewStack = InventoryContents.Count + CountToAdd;
			if (InventoryContents.ItemInformation->CanStack(NewStack))
			{
				UE_LOG(LogInventory, Log, TEXT("Stacked item %s in inventory"), *Item.ItemInformation->DisplayName.ToString());
				CountToAdd = InventoryContents.AddToStack(Item.Count);
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
		Inventory.Emplace(Item);
		OutItemHandle = Item.ItemHandle;
		OnRep_Inventory();
		return true;
	}
	else
	{
		UE_LOG(LogInventory, Warning, TEXT("Failed to add item %s to inventory"), *Item.ItemInformation->DisplayName.ToString());
	}
	
	return false;
}

bool UInventoryComponent::CanAddToInventory(const FInventoryContents& Item) const
{
	return true;
}

void UInventoryComponent::GetInventory(TArray<FInventoryContents>& OutInventory) const
{
	OutInventory = Inventory;
}

void UInventoryComponent::SetInventory(const TArray<FInventoryContents>& NewInventory)
{
	Inventory = NewInventory;
	OnRep_Inventory();
}

FItemHandle UInventoryComponent::GenerateUniqueHandle() const
{
	int UniqueID = 0;
	bool bIDTaken = true;

	while (bIDTaken)
	{
		bIDTaken = false;
		for (const auto& InventoryContents : Inventory)
		{
			if (InventoryContents.ItemHandle.HandleID == UniqueID)
			{
				UniqueID++;
				bIDTaken = true;
				break;
			}
		}
	}
	return FItemHandle(UniqueID);
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
	OnInventoryChange.Broadcast(Inventory);
}

