// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInteract, const AActor*, Interactable, const AActor*, Interactor, const USceneComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFinishInteract, const AActor*, Interactable, const AActor*, Interactor, const USceneComponent*, Component);

UENUM(BlueprintType)
enum class EInteractType : uint8
{
	SingleUse,
	Continuous
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractableComponent(const FObjectInitializer& InObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FInteract OnInteract;

	UPROPERTY(BlueprintAssignable)
	FFinishInteract OnFinishInteract;

	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	EInteractType InteractType = EInteractType::SingleUse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction")
	bool bIsPlayerInteractable = true;

	// Perform an interaction on the interactable
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void Interact(const AActor* Interactor, USceneComponent* Component);

	// Declares an interaction as finished, used primarily for continuous interaction
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void FinishInteract(const AActor* Interactor, USceneComponent* Component);

	UFUNCTION(BlueprintCallable, Category="Interaction")
	virtual bool IsPlayerInteractable() const;

	UFUNCTION(BlueprintCallable, Category="Interaction")
	EInteractType GetInteractType() const;
};
