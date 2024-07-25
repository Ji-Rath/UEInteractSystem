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

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Sets default values for this component's properties
	UToggleInteractableComponent();

	UFUNCTION(BlueprintCallable)
	void SetState(AActor* Interactor, bool bNewState);

	void SetState(bool bNewState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetState();

	UFUNCTION(BlueprintCallable)
	void ToggleState();

	void ToggleState(AActor* Interactor);

	virtual void PerformInteraction(AActor* Interactor, UPrimitiveComponent* Component) override;

	UPROPERTY(EditAnywhere, Replicated, SaveGame)
	bool bIsOn = false;
};