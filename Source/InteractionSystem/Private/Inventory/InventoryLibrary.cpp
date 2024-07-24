// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryLibrary.h"

#include "Interaction/ItemData.h"
#include "Inventory/InventoryInfo.h"

bool UInventoryLibrary::IsA(const FInventoryContents& Item, UItemInformation* Source)
{
	return Item.ItemInformation->IsA(Source->StaticClass());
}
