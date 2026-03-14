// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemVisual.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

class UItemAction;
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
	}
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
	// The display name of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	FText DisplayName;

	// The description of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	FText Description;

	/** Texture of item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI", meta = (AssetBundles = "UI"))
	TSoftObjectPtr<UTexture2D> ItemIcon = nullptr;
	
	// Whether we want to override the actor used for item visualization
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual")
	bool bOverrideActor = false;

	// The actor used for the item - ex. visualization
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual", meta = (EditCondition = "bOverrideActor", EditConditionHides, AssetBundles = "Visual"))
	TSoftClassPtr<AItemVisual> ActorClass = AItemVisual::StaticClass();
	
	// The mesh used for visualizing the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual", meta = (EditCondition = "!bOverrideActor", EditConditionHides, AssetBundles = "Visual"))
	TSoftObjectPtr<UStaticMesh> ItemMesh;
	
	// Arbitrary vector value, used in the interaction system to align equipped items so they fit the screen properly
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual|Advanced")
	FTransform ItemOffset;
	
	// Action executed when this item is used
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemInfo", Instanced)
	TObjectPtr<UItemAction> Action;

	// Determines how the item will stack when there is more than 1 in an inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemInfo")
	EStackableType StackableType;

	// The max stack possible for the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemInfo", meta=(EditCondition="StackableType == EStackableType::Stackable", EditConditionHides))
	int32 MaxStack;
	
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
