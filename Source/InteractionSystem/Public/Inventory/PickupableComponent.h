// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interaction/InteractableComponent.h"
#include "PickupableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUsePickupable, AActor*, User);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UPickupableComponent : public UInteractableComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPickupableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/** Add item to interactors inventory */
	UFUNCTION(BlueprintCallable)
	void PickupItem(AActor* Interactor);
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FUsePickupable OnUsePickupable;

	UPROPERTY(EditAnywhere, Category = "Pickupable")
	int Amount = 1;

	FTimerHandle DestroyTimer;
};
