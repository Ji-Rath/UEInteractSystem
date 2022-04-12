// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

#include "InteractionSystem_Settings.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Inventory/PickupableComponent.h"
#include "Inventory/PlayerEquipComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInventoryComponent::DropItem(const FInventoryContents& Item)
{
	APawn* Player = GetOwner<APawn>();
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* DroppedItem = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), GetOwner()->GetActorLocation(), FRotator::ZeroRotator);

	RemoveFromInventory(Item);
}

void UInventoryComponent::RemoveFromInventory_Implementation(const FInventoryContents& Item)
{
	/** If there is an item at the slot, remove specified amount */
	int Slot = FindItemSlot(Item);
	if (Inventory.IsValidIndex(Slot))
	{
		Inventory[Slot].Count -= Item.Count;
		if (Inventory[Slot].Count <= 0)
			Inventory.RemoveAt(Slot);
		OnInventoryChange.Broadcast(false);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Unable to remove item %s from inventory!"), *(Item.RowName.ToString()));
	}
}

int UInventoryComponent::FindItemSlot(const FInventoryContents& Item) const
{
	/** Find slot with item in it */
	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (Item == Inventory[i])
		{
			return i;
		}
	}
	/** Search failed */
	return -1;
}

FInventoryContents UInventoryComponent::FindItem(const int Index) const
{
	if (Inventory.IsValidIndex(Index))
	{
		return Inventory[Index];
	}

	return FInventoryContents();
}

bool UInventoryComponent::AddToInventory_Implementation(const FInventoryContents& Item)
{
	for (int i=0;i<Inventory.Num();i++)
	{
		FInventoryContents& InventoryContent = Inventory[i];
		FItemInfo* ItemInfo = Item.GetRow<FItemInfo>("");
		/** Compare names to see if they are the same item */
		if (InventoryContent.RowName == Item.RowName)
		{
			/** Ensure that adding the item will not exceed the max stack */
			if (ItemInfo && ItemInfo->CanStack(InventoryContent.Count + Item.Count))
			{
				InventoryContent.Count += Item.Count;
				OnInventoryChange.Broadcast(true);
				return true;
			}
		}
	}

	if (Inventory.Num() < InventorySize)
	{
		/** Just add the item to a new slot */
		FInventoryContents* InventoryItem = new FInventoryContents(Item);

		int Slot = Inventory.Add(*InventoryItem);
		OnInventoryChange.Broadcast(true);

		return true;
	}
	
	return false;
}

void UInventoryComponent::GetInventory(TArray<FInventoryContents>& OutInventory) const
{
	OutInventory = Inventory;
}

void UInventoryComponent::SetInventory(const TArray<FInventoryContents>& NewInventory)
{
	Inventory = NewInventory;

	OnInventoryChange.Broadcast(true);
}

