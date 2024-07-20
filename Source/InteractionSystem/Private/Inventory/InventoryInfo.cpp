// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryInfo.h"
#include "Interaction/ItemData.h"



int FInventoryContents::AddToStack(int Amount)
{
	int Remaining = (Count + Amount) - ItemInformation->GetMaxStack();
	Count = FMath::Min(Amount, ItemInformation->GetMaxStack());
	return FMath::Max(Remaining, 0);
}
