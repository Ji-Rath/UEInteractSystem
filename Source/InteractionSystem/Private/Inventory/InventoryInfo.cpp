// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryInfo.h"
#include "Interaction/ItemData.h"
#include "Inventory/InventoryComponent.h"


bool FItemData::HasRoom() const
{
	return Count < ItemInformation->GetMaxStack();
}

int FItemData::FixCount()
{
	int Remainder = FMath::Max(Count - ItemInformation->GetMaxStack(), 0);
	Count = FMath::Clamp(Count, 0, ItemInformation->GetMaxStack());
	return Remainder;
}

int FItemData::AddToStack(int Amount)
{
	if (!ensureMsgf(Amount > 0, TEXT("Amount cannot be negative!"))) { return Amount; }
	
	int Remaining = (Count + Amount) - ItemInformation->GetMaxStack();
	Count = FMath::Clamp(Count + Amount, 0, ItemInformation->GetMaxStack());
	return FMath::Max(Remaining, 0);
}

int FItemData::RemoveFromStack(int Amount)
{
	if (!ensureMsgf(Amount > 0, TEXT("Amount cannot be negative!"))) { return Amount; }
	
	int Remaining = FMath::Min(Count - Amount, 0);
	Count = FMath::Clamp(Count - Amount, 0, ItemInformation->GetMaxStack());
	return FMath::Abs(Remaining);
}

FInventoryContents::FInventoryContents(const FItemHandle& NewHandle, const TInstancedStruct<FItemData> NewItem)
: ItemHandle(NewHandle), ItemData(NewItem)
{
}

int FInventoryContents::AddToStack(int Amount)
{
	FItemData& Data = ItemData.GetMutable();
	return Data.AddToStack(Amount);
}

int FInventoryContents::RemoveFromStack(int Amount)
{
	FItemData& Data = ItemData.GetMutable();
	return Data.RemoveFromStack(Amount);
}

int FInventoryContents::FixCount()
{
	FItemData& Data = ItemData.GetMutable();
	return Data.FixCount();
}

bool FInventoryContents::HasRoom() const
{
	const FItemData& Data = ItemData.Get();
	return Data.HasRoom();
}

void FInventoryContents::PreReplicatedRemove(const FFastArraySerializer& Serializer)
{
	ItemHandle.OwningInventory->OnItemRemove.Broadcast(*this);
}

void FInventoryContents::PostReplicatedAdd(const FFastArraySerializer& Serializer)
{
	ItemHandle.OwningInventory->OnItemAdd.Broadcast(*this);
}

void FInventoryContents::PostReplicatedChange(const FFastArraySerializer& Serializer)
{
	ItemHandle.OwningInventory->OnItemChange.Broadcast(*this);
}
