// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionLibrary.h"

#include "Inventory/InventoryInfo.h"

FName UInteractionLibrary::GetRowName(const FInventoryContents& Item)
{
	return Item.RowHandle.IsNull() ? Item.RowName : Item.RowHandle.RowName;
}
