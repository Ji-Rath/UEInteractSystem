// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryInfo.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FInventoryContents
{
	GENERATED_USTRUCT_BODY()

	// Used to easily get data table row in editor
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle RowHandle;

	// Primarily used for serialization, should be identical to RowHandle RowName
	UPROPERTY(SaveGame)
	FName RowName;

	// String path to Table, used for serialization
	UPROPERTY(SaveGame)
	FString TableString;

	// The current item count in the stack
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	int Count;

	FInventoryContents()
	{
		Count = 0;
	}

	FInventoryContents(FDataTableRowHandle Item, int Amount)
	{
		Count = Amount;
	}

	FInventoryContents(FDataTableRowHandle Item)
	{
		Count = 1;
	}

	// In order to support serialization, object pointers should be converted to string paths. Due to limitations with FDataTableRowHandle, variables need to
	// be moved outside the struct. Its really only being kept due to the convenience of setting values in the editor
	static void PrepareSerialization(FInventoryContents& objToSerialize)
	{
		const TSoftObjectPtr<UDataTable> DataTable = objToSerialize.RowHandle.DataTable;
		if (DataTable.IsValid())
		{
			objToSerialize.RowName = objToSerialize.RowHandle.RowName;
			objToSerialize.TableString = DataTable.ToSoftObjectPath().ToString();
		}
	}
	
	bool operator==(const FInventoryContents& OtherSlot) const
	{
		const FName Name = GetRowName();
		const FName OtherName = OtherSlot.GetRowName();
		return Name == OtherName;
	}

	FName GetRowName() const
	{
		return RowName.IsNone() ? RowHandle.RowName : RowName;
	}
	
	friend FArchive& operator<<(FArchive& Ar, FInventoryContents& objToSerialize) {
		PrepareSerialization(objToSerialize);
		Ar << objToSerialize.Count;
		Ar << objToSerialize.TableString;
		Ar << objToSerialize.RowName;
		return Ar;
	}

	/** Get the row straight from the row handle */
	template <class T>
	T* GetRow(const TCHAR* ContextString) const
	{
		if (!RowHandle.IsNull())
		{
			return RowHandle.GetRow<T>("");
		}
		
		const TSoftObjectPtr<UDataTable> DataTable = TSoftObjectPtr<UDataTable>(TableString);
		if (DataTable.IsValid())
		{
			return DataTable->FindRow<T>(RowName, ContextString);
		}
		
		return nullptr;
	}

	template <class T>
	T* GetRow(const FString& ContextString) const
	{
		return GetRow<T>(*ContextString);
	}

	/** Returns true if this handle is specifically pointing to nothing */
	bool IsNull() const
	{
		return TableString.IsEmpty() && RowHandle.IsNull();
	}
};
