// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InstancedStruct.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "StructUtils/InstancedStruct.h"
#include "InventoryInfo.generated.h"

class UInventoryComponent;
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

	bool operator==(const FItemHandle& ItemHandle) const
	{
		return HandleID == ItemHandle.HandleID;
	}

	bool IsValid() const
	{
		return HandleID != -1;
	}
};

/**
 * Item data which is dynamically stored in a InventoryComponent
 * Can be extended to allow for additional parameters if needed
 */
USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	// Used to easily get data table row in editor. Is not reliable to give accurate info when playing in-game
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UItemInformation* ItemInformation;

	// The current item count in the stack
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	int Count;

	virtual bool operator==(const FItemData& ItemData) const
	{
		return ItemInformation == ItemData.ItemInformation;
	}
	
	virtual bool IsValid() const
	{
		return ItemInformation && Count > 0;
	}

	virtual bool HasRoom() const;

	virtual int FixCount();
	int AddToStack(int Amount);
	int RemoveFromStack(int Amount);
};

/** Holds information about an item */
USTRUCT(Blueprintable, BlueprintType)
struct FInventoryContents : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInventoryContents() {};
	FInventoryContents(const FItemHandle& NewHandle, const TInstancedStruct<FItemData> NewItem, UInventoryComponent* NewOwner);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FItemHandle ItemHandle;
	
	// Dynamic data that will vary from item to item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TInstancedStruct<FItemData> Item;

	UPROPERTY()
	UInventoryComponent* OwnerComp;

	template<typename T>
	T* GetItemInformation() const
	{
		return Cast<T>(Item.Get<FItemData>().ItemInformation);
	}

	// Compare data (besides count) to see if two items are the same
	bool operator==(const FInventoryContents& OtherSlot) const
	{
		return Item.Get<FItemData>() == OtherSlot.Item.Get<FItemData>();
	}

	// Compare item handles to see if two items are the same
	bool operator==(const FItemHandle& OtherItemHandle) const
	{
		return ItemHandle == OtherItemHandle;
	}

	// Compare static data to see if two items are the same
	bool operator==(const UItemInformation* OtherItemInfo) const
	{
		return Item.Get<FItemData>().ItemInformation == OtherItemInfo;
	}

	bool operator==(const FItemData& OtherItemInfo) const
	{
		return Item.Get<FItemData>().ItemInformation == OtherItemInfo.ItemInformation;
	}
	
	friend FArchive& operator<<(FArchive& Ar, FInventoryContents& objToSerialize)
	{
		//Ar << objToSerialize.Count;
		return Ar;
	}

	/**
	 * Attempt to add items to the stack
	 * @param Amount The amount to add
	 * @return The amount that could not be added to the stack
	 */
	int AddToStack(int Amount);
	int RemoveFromStack(int Amount);
	int FixCount();
	bool HasRoom() const;

	bool IsValid() const
	{
		auto ItemData = Item.GetPtr<FItemData>();
		return ItemData && ItemData->IsValid();
	}
	
	void PreReplicatedRemove(const FFastArraySerializer& Serializer);
	void PostReplicatedAdd(const FFastArraySerializer& Serializer);
	void PostReplicatedChange(const FFastArraySerializer& Serializer);
};

USTRUCT(Blueprintable, BlueprintType)
struct FInventoryContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FInventoryContents> Items;
	
	/** Step 4: Copy this, replace example with your names */
	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FastArrayDeltaSerialize<FInventoryContents>(Items, DeltaParms, *this);
	}
};

template<>
struct TStructOpsTypeTraits<FInventoryContainer> : public TStructOpsTypeTraitsBase2<FInventoryContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
