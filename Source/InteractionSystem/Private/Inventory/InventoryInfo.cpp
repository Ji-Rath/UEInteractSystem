// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryInfo.h"
#include "Interaction/ItemData.h"
#include "Inventory/InventoryComponent.h"


int FInventoryContents::AddToStack(int Amount)
{
	int Remaining = (Count + Amount) - ItemInformation->GetMaxStack();
	Count = FMath::Min(Amount, ItemInformation->GetMaxStack());
	return FMath::Max(Remaining, 0);
}

void FInventoryContents::PreReplicatedRemove(const FFastArraySerializer& Serializer)
{
	auto InvComp = Cast<UInventoryComponent>(Serializer.OwningObject);
	InvComp->OnItemRemove.Broadcast(*this);
}

void FInventoryContents::PostReplicatedAdd(const FFastArraySerializer& Serializer)
{
	auto InvComp = Cast<UInventoryComponent>(Serializer.OwningObject);
	InvComp->OnItemAdd.Broadcast(*this);
}

void FInventoryContents::PostReplicatedChange(const FFastArraySerializer& Serializer)
{
	auto InvComp = Cast<UInventoryComponent>(Serializer.OwningObject);
	InvComp->OnItemChange.Broadcast(*this);
}
