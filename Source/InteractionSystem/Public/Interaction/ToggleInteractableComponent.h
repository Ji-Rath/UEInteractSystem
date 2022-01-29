// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "Components/ActorComponent.h"
#include "ToggleInteractableComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UToggleInteractableComponent : public UInteractableComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UToggleInteractableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief Set the state of the interactable
	 * @param Interactor The interactor who set the state of the interactable
	 * @param bNewState The new state of the interactable
	 */
	UFUNCTION(BlueprintCallable)
	void SetState(AActor* Interactor, bool bNewState);

	/**
	 * @brief Gets the state of the interactable
	 * @return The current state of the interactable
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetState() const;

	/**
	 * @brief Toggle the state of the interactable
	 */
	UFUNCTION(BlueprintCallable)
	void ToggleState();
	
	/**
	 * @brief Internal function used when OnFinishInteract is called
	 * @param bShouldToggle Whether the ToggleInteractable should inverse states
	 */
	void ToggleState(bool bShouldToggle);

	bool bIsOn = false;
};
