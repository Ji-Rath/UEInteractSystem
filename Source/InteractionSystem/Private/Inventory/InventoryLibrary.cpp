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

const TInstancedStruct<FItemData>& UInventoryLibrary::GetItemDataByHandle(const FItemHandle& ItemHandle)
{
	check(ItemHandle.OwningInventory)
	
	return ItemHandle.OwningInventory->GetItemDataPtr(ItemHandle)->ItemData;
}

TInstancedStruct<FItemData>& UInventoryLibrary::GetItemDataMutableByHandle(const FItemHandle& ItemHandle)
{
	check(ItemHandle.OwningInventory)
	
	return ItemHandle.OwningInventory->GetItemDataPtr(ItemHandle)->ItemData;
}

void UInventoryLibrary::AddItemState(const FItemHandle& ItemHandle, FGameplayTagContainer InState)
{
	TInstancedStruct<FItemData>& ItemData = GetItemDataMutableByHandle(ItemHandle);
	ItemData.GetMutable().ItemState.AppendTags(InState);
}

void UInventoryLibrary::RemoveItemState(const FItemHandle& ItemHandle, FGameplayTagContainer InState)
{
	TInstancedStruct<FItemData>& ItemData = GetItemDataMutableByHandle(ItemHandle);
	ItemData.GetMutable().ItemState.RemoveTags(InState);
}

void UInventoryLibrary::SetItemAttribute(const FItemHandle& ItemHandle, const FItemAttribute& InAttribute)
{
	TInstancedStruct<FItemData>& ItemData = GetItemDataMutableByHandle(ItemHandle);
	
	// Override value or add attribute if it doesnt exist
	if (FItemAttribute* Item = ItemData.GetMutable().ItemAttributes.FindByKey(InAttribute))
	{
		Item->Value = InAttribute.Value;
	}
	else
	{
		ItemData.GetMutable().ItemAttributes.Add(InAttribute);
	}
}

bool UInventoryLibrary::DoesItemHaveState(const FItemHandle& ItemHandle, FGameplayTagContainer InState, bool bCheckExact)
{
	TInstancedStruct<FItemData>& ItemData = GetItemDataMutableByHandle(ItemHandle);
	return bCheckExact ? ItemData.GetMutable().ItemState.HasAllExact(InState) : ItemData.GetMutable().ItemState.HasAll(InState);
}

bool UInventoryLibrary::DoesItemHaveAttribute(const FItemHandle& ItemHandle, FGameplayTag InAttributeTag)
{
	TInstancedStruct<FItemData>& ItemData = GetItemDataMutableByHandle(ItemHandle);
	return ItemData.GetMutable().ItemAttributes.FindByKey(InAttributeTag) != nullptr;
}

float UInventoryLibrary::GetItemAttributeValue(const FItemHandle& ItemHandle, FGameplayTag InAttributeTag)
{
	TInstancedStruct<FItemData>& ItemData = GetItemDataMutableByHandle(ItemHandle);
	FItemAttribute* ItemAttribute = ItemData.GetMutable().ItemAttributes.FindByKey(InAttributeTag);
	
	return ItemAttribute ? ItemAttribute->Value : 0.0f;
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
