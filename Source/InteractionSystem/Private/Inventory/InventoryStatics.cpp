// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryStatics.h"

#include "Inventory/InventoryInfo.h"

bool UInventoryStatics::IsItemValid(FItemHandle& ItemHandle)
{
	return ItemHandle.IsValid();
}
