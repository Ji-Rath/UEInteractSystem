// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

class UStaticMesh;
class UTexture;
/**
 * Data Asset that stores essential values for an interactable, more specifically a pickupable
 */
UCLASS()
class INTERACTIONSYSTEM_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxStack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ActorClass;

	/** Compare classes when comparing ItemData */
	bool operator==(const UItemData& OtherItem) const
	{
		return GetClass() == OtherItem.GetClass();
	}

	UItemData() {
		Name = FText::FromString("NoName");
		Description = FText::FromString("NoDesc");
		MaxStack = 1;
	}
};

UENUM()
enum class EStackableType
{
	NoStacking,
	Stackable,
	InfiniteStack
};

UENUM()
enum class EItemType
{
	Item,
	Readable
};

/** Structure that defines a level up table entry */
USTRUCT(BlueprintType)
struct FItemInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FItemInfo()
	{
		DisplayName = FText::FromString(TEXT("NoDesc"));
		Description = FText::FromString(TEXT("NoDesc"));
		StackableType = EStackableType::NoStacking;
		MaxStack = 1;
		ItemMesh = nullptr;
		ItemType = EItemType::Item;
	}

	/** The 'Name' column is the same as the XP Level */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Info)
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Info)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Info)
	TSoftObjectPtr<UStaticMesh> ItemMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	EStackableType StackableType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data)
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data, meta=(EditCondition="StackableType == EStackableType::Stackable", EditConditionHides))
	int32 MaxStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Data, meta=(EditCondition="ItemType == EItemType::Readable", EditConditionHides))
	TArray<FText> PageData;
	
	bool CanStack(int Count) const
	{
		return StackableType == EStackableType::InfiniteStack || (StackableType == EStackableType::Stackable && Count <= MaxStack);
	}

	bool operator!=(const FItemInfo& ItemInfo) const
	{
		return !ItemInfo.DisplayName.IdenticalTo(DisplayName);
	}
};
