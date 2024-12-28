// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryInfo.h"
#include "Interaction/ItemData.h"
#include "Inventory/InventoryComponent.h"


int FInventoryContents::AddToStack(int Amount)
{
	if (!ensureMsgf(Amount > 0, TEXT("Amount cannot be negative!"))) { return Amount; }
	
	int Remaining = (Count + Amount) - ItemInformation->GetMaxStack();
	Count = FMath::Clamp(Count + Amount, 0, ItemInformation->GetMaxStack());
	return FMath::Max(Remaining, 0);
}

int FInventoryContents::RemoveFromStack(int Amount)
{
	if (!ensureMsgf(Amount > 0, TEXT("Amount cannot be negative!"))) { return Amount; }
	
	int Remaining = FMath::Min(Count - Amount, 0);
	Count = FMath::Clamp(Count - Amount, 0, ItemInformation->GetMaxStack());
	return FMath::Abs(Remaining);
}

int FInventoryContents::FixCount()
{
	int Remainder = FMath::Max(Count - ItemInformation->GetMaxStack(), 0);
	Count = FMath::Clamp(Count, 0, ItemInformation->GetMaxStack());
	return Remainder;
}

bool FInventoryContents::HasRoom() const
{
	return Count < ItemInformation->GetMaxStack();
}

void FInventoryContents::PreReplicatedRemove(const FFastArraySerializer& Serializer)
{
	auto InvComp = Cast<UInventoryComponent>(OwnerComp);
	InvComp->OnItemRemove.Broadcast(*this);
}

void FInventoryContents::PostReplicatedAdd(const FFastArraySerializer& Serializer)
{
	auto InvComp = Cast<UInventoryComponent>(OwnerComp);
	InvComp->OnItemAdd.Broadcast(*this);
}

void FInventoryContents::PostReplicatedChange(const FFastArraySerializer& Serializer)
{
	auto InvComp = Cast<UInventoryComponent>(OwnerComp);
	InvComp->OnItemChange.Broadcast(*this);
}
