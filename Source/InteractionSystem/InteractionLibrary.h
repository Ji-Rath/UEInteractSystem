// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIONSYSTEM_API UInteractionLibrary : public UBlueprintFunctionLibrary
{
 GENERATED_BODY()
public:
 UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
 static FName GetRowName(const FInventoryContents& Item);
};
