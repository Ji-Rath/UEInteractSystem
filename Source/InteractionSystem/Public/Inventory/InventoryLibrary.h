// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryLibrary.generated.h"

class AItemVisual;
struct FItemHandle;
class UItemInformation;
struct FInventoryContents;

DECLARE_DYNAMIC_DELEGATE_OneParam(FItemActorSpawned, AItemVisual*, Actor);
/**
 * 
 */
UCLASS()
class INTERACTIONSYSTEM_API UInventoryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Returns whether the specified item is an instance of the specified source
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static bool IsA(const FInventoryContents& Item, UItemInformation* Source);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static bool IsValid(const FItemHandle& ItemHandle);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static const TInstancedStruct<FItemData>& GetItemDataByHandle(const FItemHandle& ItemHandle);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	static void AddItemState(const FItemHandle& ItemHandle, FGameplayTagContainer InState);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	static void RemoveItemState(const FItemHandle& ItemHandle, FGameplayTagContainer InState);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	static void SetItemAttribute(const FItemHandle& ItemHandle, const FItemAttribute& InAttribute);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static bool DoesItemHaveState(const FItemHandle& ItemHandle, FGameplayTagContainer InState, bool bCheckExact = false);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static bool DoesItemHaveAttribute(const FItemHandle& ItemHandle, FGameplayTag InAttributeTag);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static float GetItemAttributeValue(const FItemHandle& ItemHandle, FGameplayTag InAttributeTag);
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (ItemHandle)", CompactNodeTitle = "==", ScriptMethod = "Equals", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Item")
	static bool EqualEqual_ItemHandleItemHandle(const FItemHandle& A, const FItemHandle& B);
	
	/**
	 * Spawn an item into the world using the ActorClass stored in the ItemInformation DataAsset
	 * @param WorldContextObject The World Context
	 * @param ItemInformation The Item that we want to spawn an actor of
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = WorldContextObject), Category="Inventory")
	static AItemVisual* SpawnItem(const UObject* WorldContextObject, UItemInformation* ItemInformation);
};
