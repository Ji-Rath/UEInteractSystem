// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteract, AActor*, Interactor, UPrimitiveComponent*, Component);

UCLASS(Blueprintable, ClassGroup=(Interactable), meta=(BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractableComponent();
	
	UFUNCTION(BlueprintPure, BlueprintCallable, BlueprintNativeEvent, Category="Interactable")
	bool CanInteract(AActor* Interactor, UPrimitiveComponent* Component);

	// Perform an interaction on the actor
	UFUNCTION(BlueprintCallable, Category="Interactable")
	void Interact(AActor* Interactor, UPrimitiveComponent* Component = nullptr);
	
	/* Called when the interactable component is interacted with */
	UPROPERTY(BlueprintAssignable, Category="Interactable")
	FInteract OnInteract;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category="Interactable")
	FText GetName(UPrimitiveComponent* Component = nullptr) const;

	/* Convenient function for broadcasting interact messages */
	UFUNCTION(BlueprintCallable, Category="Interactable")
	void SendInteractMessage(AActor* Interactor, FText Message);

protected:
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* Interactor, UPrimitiveComponent* Component);

	virtual void PerformInteraction(AActor* Interactor, UPrimitiveComponent* Component);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interactable")
	FText DefaultName;
};
