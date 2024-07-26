// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InstancedStruct.h"
#include "InventoryInfo.generated.h"

class UItemInformation;

// Handle for an item
USTRUCT(BlueprintType)
struct FItemHandle
{
	GENERATED_BODY()

	UPROPERTY()
	int HandleID = -1;
	
	FItemHandle(int ID = -1) : HandleID(ID) {}

	virtual bool IsValid()
	{
		return HandleID != -1;
	}

	bool operator==(const FItemHandle& ItemHandle) const = default;

	bool IsValid() const
	{
		return HandleID != -1;
	}
};

/** Holds information about an item */
USTRUCT(Blueprintable, BlueprintType)
struct FInventoryContents
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FItemHandle ItemHandle;

	// Used to easily get data table row in editor. Is not reliable to give accurate info when playing in-game
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UItemInformation* ItemInformation;

	// The current item count in the stack
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	int Count;

	// Dynamic data that will vary from item to item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FInstancedStruct DynamicData;

	template<typename T>
	T* GetItemInformation() const
	{
		return Cast<T>(ItemInformation);
	}

	FInventoryContents()
	{
		Count = 0;
		ItemInformation = nullptr;
	}

	FInventoryContents(UItemInformation* Item, int Amount)
	{
		ItemInformation = Item;
		Count = Amount;
	}

	FInventoryContents(UItemInformation* Item, int Amount, const FInstancedStruct& InDynamicData)
	{
		ItemInformation = Item;
		Count = Amount;
		DynamicData = InDynamicData;
	}

	FInventoryContents(UItemInformation* Item)
	{
		ItemInformation = Item;
		Count = 1;
	}

	// Compare data (besides count) to see if two items are the same
	bool operator==(const FInventoryContents& OtherSlot) const
	{
		return ItemInformation == OtherSlot.ItemInformation && DynamicData == OtherSlot.DynamicData;
	}

	// Compare item handles to see if two items are the same
	bool operator==(const FItemHandle& OtherItemHandle) const
	{
		return ItemHandle == OtherItemHandle;
	}

	// Compare static data to see if two items are the same
	bool operator==(const UItemInformation* OtherItemInfo) const
	{
		return ItemInformation == OtherItemInfo;
	}
	
	friend FArchive& operator<<(FArchive& Ar, FInventoryContents& objToSerialize)
	{
		Ar << objToSerialize.Count;
		return Ar;
	}

	/**
	 * Attempt to add items to the stack
	 * @param Amount The amount to add
	 * @return The amount that could not be added to the stack
	 */
	int AddToStack(int Amount);

	bool IsValid() const
	{
		return Count > 0 && ItemInformation;
	}
};
