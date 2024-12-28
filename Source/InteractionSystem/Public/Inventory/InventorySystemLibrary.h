// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventorySystemLibrary.generated.h"

struct FItemHandle;
/**
 * 
 */
UCLASS()
class INTERACTIONSYSTEM_API UInventorySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (ItemHandle)", CompactNodeTitle = "==", ScriptMethod = "Equals", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|Item")
	static bool EqualEqual_ItemHandleItemHandle(const FItemHandle& A, const FItemHandle& B);
};
