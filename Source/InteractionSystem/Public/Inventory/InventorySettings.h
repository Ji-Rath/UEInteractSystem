// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InventorySettings.generated.h"

class APickupable;
/**
 * Manage settings related to the inventory system
 */
UCLASS(Config="Game", DefaultConfig)
class INTERACTIONSYSTEM_API UInventorySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config)
	TSoftClassPtr<APickupable> DefaultPickupable;
};
