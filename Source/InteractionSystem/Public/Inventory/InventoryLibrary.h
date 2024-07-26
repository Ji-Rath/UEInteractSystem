// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryLibrary.generated.h"

struct FItemHandle;
class UItemInformation;
struct FInventoryContents;
/**
 * 
 */
UCLASS()
class INTERACTIONSYSTEM_API UInventoryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// Returns whether the specified item is an instance of the specified source
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static bool IsA(const FInventoryContents& Item, UItemInformation* Source);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static bool IsValid(FItemHandle& ItemHandle);
};
