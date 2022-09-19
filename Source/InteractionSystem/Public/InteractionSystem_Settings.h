// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Object.h"
#include "InteractionSystem_Settings.generated.h"

/**
 * 
 */
UCLASS(Config = MySetting)
class INTERACTIONSYSTEM_API UInteractionSystem_Settings : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, Category = "Inventory")
	TSoftObjectPtr<UDataTable> ItemTable;
};
