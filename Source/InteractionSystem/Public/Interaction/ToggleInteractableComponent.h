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

	UFUNCTION(BlueprintCallable)
	void SetState(AActor* Interactor, bool bNewState);

	void SetState(bool bNewState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetState();

	UFUNCTION(BlueprintCallable)
	void ToggleState();

	void ShouldToggleState(bool bShouldToggle);

	void ToggleState(AActor* Interactor);

	UPROPERTY(EditAnywhere, SaveGame)
	bool bIsOn = false;

	/* Determines whether you want the state of the interactable to change automatically or be manually managed by calling OnFinishInteraction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoManageState = true;
};