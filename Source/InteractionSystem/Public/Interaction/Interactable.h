// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UENUM(BlueprintType)
enum class EInteractType : uint8
{
	SingleUse,
	Continuous
};

// This class does not need to be modified.
UINTERFACE()
class INTERACTIONSYSTEM_API UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Defines an actor as being interactable by a user
 */
class INTERACTIONSYSTEM_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Perform an interaction on the interactable
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Interaction")
	void Interact(const AActor* Interactor, USceneComponent* Component);

	// Declares an interaction as finished, used primarily for continuous interaction
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Interaction")
	void FinishInteract(const AActor* Interactor, USceneComponent* Component);

	// Returns whether the actor can be interacted with
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Interaction")
	bool CanInteract(const AActor* Interactor, USceneComponent* Component) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Interaction")
	FText GetName(USceneComponent* Component) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Interaction")
	EInteractType GetInteractType() const;
};
