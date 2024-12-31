// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "StructUtils/InstancedStruct.h"
#include "ItemData.generated.h"

class UStaticMesh;
class UTexture;

/** Enum that defines the type of stacking that an item can have */
UENUM()
enum class EStackableType : uint8
{
	// The item cannot be stacked
	NoStacking,
	// The item can be stacked up to the MaxStack value
	Stackable,
	// The item can be infinitely stacked
	InfiniteStack
};

USTRUCT(BlueprintType)
struct FDataAssetWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataAsset* DataAsset;
};

/** Holds information about an item */
UCLASS(Blueprintable, BlueprintType)
class INTERACTIONSYSTEM_API UItemInformation : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UItemInformation()
	{
		DisplayName = FText::FromString(TEXT("NoName"));
		Description = FText::FromString(TEXT("NoDesc"));
		StackableType = EStackableType::NoStacking;
		MaxStack = 1;
		ItemMesh = nullptr;
	}
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
	// The display name of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	FText DisplayName;

	// The description of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	FText Description;

	/** Texture of item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visual")
	UTexture2D* ItemIcon = nullptr;

	// Determines how the item will stack when there is more than 1 in an inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	EStackableType StackableType;

	// Whether the item uses a custom class or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual")
	bool bCustomClass = false;

	// The custom actor used for the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual", meta=(EditCondition="bCustomClass == true", EditConditionHides))
	TSubclassOf<AActor> CustomClass;

	// The mesh used for the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual", meta=(EditCondition="bCustomClass == false", EditConditionHides))
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	// The max stack possible for the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data", meta=(EditCondition="StackableType == EStackableType::Stackable", EditConditionHides))
	int32 MaxStack;

	// Arbitrary vector value, used in the interaction system to align equipped items so they fit the screen properly
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual|Optional")
	FTransform ItemOffset;
	
	bool CanStack(int Count) const
	{
		return StackableType == EStackableType::InfiniteStack || (StackableType == EStackableType::Stackable && Count <= MaxStack);
	}

	int GetMaxStack() const
	{
		switch(StackableType)
		{
			case EStackableType::NoStacking:
				return 1;
			case EStackableType::Stackable:
				return MaxStack;
			case EStackableType::InfiniteStack:
				return INT_MAX;
			default:
				return 1;
		}
	}
};
