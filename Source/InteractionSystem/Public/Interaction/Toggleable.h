// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Toggleable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UToggleable : public UInterface
{
	GENERATED_BODY()
};

/**
 * An actor that can be toggled on and off
 */
class INTERACTIONSYSTEM_API IToggleable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Toggleable")
	void Toggle();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Toggleable")
	void SetToggle(bool bIsOn);
};
