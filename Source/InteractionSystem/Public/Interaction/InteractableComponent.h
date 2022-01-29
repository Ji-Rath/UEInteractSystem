// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteract, AActor*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFinishInteract, bool, bSuccess);

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

	/**
	 * @brief Call at the end of the interaction event to determine whether it was successful
	 * @param bSuccess Whether the interaction was successful1
	 */
	UFUNCTION(BlueprintCallable)
	void FinishInteraction(bool bSuccess);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanInteract() const;

	UFUNCTION(BlueprintCallable)
	void SetCanInteract(bool bNewCanInteract);

	UPROPERTY(EditAnywhere)
	bool bCanInteract = true;
	
	/**
	 * @brief Called when the interactable has finished its interaction event
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FFinishInteract OnFinishInteract;
	
	/**
	 * @brief Called when the interactable is interacted with
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FInteract OnInteract;

	/**
	 * @brief Stores the prim component this interactable manages
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FComponentReference PrimComponent;

	/**
	 * @brief Gets the name of the interactable, overridden by the name of ItemDataComponent if it exists
	 * @return The name of the interactable
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText GetName() const;

	/**
	 * @brief The name of the interactable
	 */
	UPROPERTY(EditAnywhere)
	FText Name;

	/**
	 * @brief Gets the current interact text for the interactable
	 * @return The current interact text
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText GetInteractText();

	/**
	 * @brief Sets the interact message to NewText
	 * @param NewText The new interact message
	 */
	UFUNCTION(BlueprintCallable)
	void SetInteractText(FText NewText);

	/**
	 * @brief Message displayed when the interactable is interacted with
	 */
	UPROPERTY(EditAnywhere)
	FText InteractText;
};
