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
	// The item cannot be stacked
	NoStacking,
	// The item can be stacked up to the MaxStack value
	Stackable,
	// The item can be infinitely stacked
	InfiniteStack
};

UENUM()
enum class EItemType
{
	// The item is a simple pickupable
	Item,
	// The item is meant to be read from the world
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
	
	// The display name of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	FText DisplayName;

	// The description of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	FText Description;

	// Determines how the item will stack when there is more than 1 in an inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	EStackableType StackableType;

	// Whether the item uses a custom class or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	bool bCustomClass = false;

	// The type of item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	EItemType ItemType;

	// The custom actor used for the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh", meta=(EditCondition="bCustomClass == true", EditConditionHides))
	TSubclassOf<AActor> CustomClass;

	// The mesh used for the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh", meta=(EditCondition="bCustomClass == false", EditConditionHides))
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	// The max stack possible for the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data", meta=(EditCondition="StackableType == EStackableType::Stackable", EditConditionHides))
	int32 MaxStack;

	// Page information, each array element signifies a page
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data", meta=(EditCondition="ItemType == EItemType::Readable", EditConditionHides, MultiLine="true"))
	TArray<FText> PageData;

	// Arbitrary vector value, used in the interaction system to align equipped items so they fit the screen properly
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Optional")
	FVector ItemOffset;
	
	bool CanStack(int Count) const
	{
		return StackableType == EStackableType::InfiniteStack || (StackableType == EStackableType::Stackable && Count <= MaxStack);
	}

	bool operator!=(const FItemInfo& ItemInfo) const
	{
		return !ItemInfo.DisplayName.IdenticalTo(DisplayName);
	}
};
