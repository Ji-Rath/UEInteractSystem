// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryStatics.generated.h"

struct FItemHandle;
/**
 * 
 */
UCLASS()
class INTERACTIONSYSTEM_API UInventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	static bool IsItemValid(FItemHandle& ItemHandle);
};
