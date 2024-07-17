// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/ItemData.h"
#include "Inventory/InventoryInfo.h"


int FInventoryContents::AddToStack(int Amount)
{
	int Remaining = (Count + Amount) - ItemInformation->GetMaxStack();
	Count = FMath::Min(Amount, ItemInformation->GetMaxStack());
	return FMath::Max(Remaining, 0);
}
