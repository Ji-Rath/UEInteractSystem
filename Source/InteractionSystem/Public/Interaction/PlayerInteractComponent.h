// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractComponent.generated.h"

class UInteractableComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateInteract, bool, bInteractable, UInteractableComponent*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerInteract, UInteractableComponent*, Interactable);

/**
 * Allows the player to interact with interactables, executing functionality based on what was interacted with
 */
UCLASS(Blueprintable, ClassGroup=(Interactable), meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UPlayerInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInteractComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Called when the player wants to interact with the currently viewed interactable
	 * @param whether the interaction was successful
	 * @return interacted actor
	*/
	UFUNCTION(BlueprintCallable, Category = "PlayerInteract")	
	UInteractableComponent* Interact();

	/**
	 * BindAction friendly version of Interact()
	 * @return void
	*/
	UFUNCTION()
	void InteractAction();

	//Delegate called when there is a change in InteractHover
	UPROPERTY(BlueprintAssignable, Category = "PlayerInteract")
	FUpdateInteract OnUpdateInteract;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FPlayerInteract OnInteract;

	UFUNCTION(BlueprintCallable, Category = "PlayerInteract")
	UInteractableComponent* GetInteractComponent(UPrimitiveComponent* PrimitiveComponent);

protected:
	UFUNCTION()
	virtual void HoverInteraction(float DeltaTime);
	
	//Store interact actor that the player is currently looking at
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerInteract")
	UInteractableComponent* HoverInteractable = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerInteract")
	UPrimitiveComponent* HoverPrimitive = nullptr;

	//Distance that the player can interact with objects
	UPROPERTY(EditDefaultsOnly, Category = "PlayerInteract")
	float InteractDistance = 500.f;
};
