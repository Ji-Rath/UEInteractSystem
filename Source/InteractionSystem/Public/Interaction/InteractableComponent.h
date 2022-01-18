// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteract, AActor*, Interactor);

UCLASS(Blueprintable, ClassGroup=(Interactable), meta=(BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, BlueprintCallable, BlueprintNativeEvent)
	bool CanInteract();

	UFUNCTION(BlueprintCallable)
	void SetCanInteract(bool bCanInteract);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FInteract OnInteract;

	/**
	 * @brief Stores the prim component this interactable manages
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FComponentReference PrimComponent;

	UFUNCTION(BlueprintCallable)
	FText GetName();

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY()
	bool bInteractable = true;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText GetInteractText();

	UFUNCTION(BlueprintCallable)
	void SetInteractText(FText NewText);

	UPROPERTY(EditAnywhere)
	FText InteractText;
};
