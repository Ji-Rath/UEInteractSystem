// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryLibrary.h"

#include "Engine/AssetManager.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryInfo.h"
#include "Item/ItemData.h"

bool UInventoryLibrary::IsA(const FInventoryContents& Item, UItemInformation* Source)
{
	return Item.ItemData.Get().ItemInformation->IsA(Source->StaticClass());
}

bool UInventoryLibrary::IsValid(const FItemHandle& ItemHandle)
{
	return ItemHandle.IsValid();
}

TInstancedStruct<FItemData> UInventoryLibrary::GetItemDataByHandle(const FItemHandle& ItemHandle)
{
	check(ItemHandle.OwningInventory)
	
	return ItemHandle.OwningInventory->GetItemByHandle(ItemHandle).ItemData;
}

bool UInventoryLibrary::EqualEqual_ItemHandleItemHandle(const FItemHandle& A, const FItemHandle& B)
{
	return A == B;
}

AItemVisual* UInventoryLibrary::SpawnItem(const UObject* WorldContextObject, UItemInformation* ItemInformation)
{
	check(ItemInformation->ActorClass.IsValid());
	
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	
	AItemVisual* ItemVisual = World->SpawnActor<AItemVisual>(ItemInformation->ActorClass.Get());
	ItemVisual->SetItem(ItemInformation);
	
	return ItemVisual;
}
