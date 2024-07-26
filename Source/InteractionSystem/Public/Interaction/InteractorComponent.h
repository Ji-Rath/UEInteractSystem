// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"

class UDEPRECATED_InteractableComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateHover, TWeakObjectPtr<UPrimitiveComponent>, Component);

/**
 * Allows the player to interact with interactables, executing functionality based on what was interacted with
 */
UCLASS(Blueprintable, ClassGroup=(Interactable), meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractorComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Called when the player wants to interact with the currently viewed interactable
	 * @param whether the interaction was successful
	 * @return interacted actor
	*/
	UFUNCTION(BlueprintCallable, Category = "PlayerInteract")
	void Interact();

	UFUNCTION(BlueprintCallable)
	void InteractWith(USceneComponent* Component);
	
	void InteractWith(AActor* Actor);

	// Called when there is a change in the currently hovered primitive
	UPROPERTY(BlueprintAssignable, Category = "PlayerInteract")
	FUpdateHover OnUpdateHover;

protected:
	UFUNCTION()
	virtual void UpdateHoverActor();

	UFUNCTION(Server, Reliable)
	void ServerInteract(USceneComponent* Component);

	UFUNCTION()
	virtual void PerformInteraction(USceneComponent* Component);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerInteract")
	TWeakObjectPtr<UPrimitiveComponent> HoverPrimitive = nullptr;

	//Distance that the player can interact with objects
	UPROPERTY(EditDefaultsOnly, Category = "PlayerInteract", meta=(Units="cm"))
	float InteractDistance = 500.f;
};
