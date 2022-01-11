// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteract, AActor *, Interactor);

class UItemData;

UCLASS()
class INTERACTIONSYSTEM_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Implementation of interaction, call Interact() to trigger function */
	UFUNCTION(BlueprintNativeEvent)
	void OnInteract(AActor* Interactor);
	virtual void OnInteract_Implementation(AActor* Interactor);

	/**
	 * Called to interact with interactable
	 * @param Interactor The actor that is trying to interact with the actor
	 * @return bool Whether the interaction was successful
	*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual bool Interact(AActor* Interactor);

	/** Returns whether the interactable can be interacted with */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanInteract(AActor* Interactor) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintImplementableEvent)
	FText GetInteractMessage();

	/** Returns the name of the interactable */
	UFUNCTION(BlueprintCallable, Category = "Interaction|Data")
	virtual FText GetName() const;

	UPROPERTY(BlueprintAssignable)
	FInteract OnInteracted;

protected:
	/** Amount of times the actor can be interacted, -1 for no limit */
	UPROPERTY(EditAnywhere, Category = "Interaction")
	int InteractAmount = -1;

	/**
	 * Name of interactable
	 */
	UPROPERTY(EditAnywhere, Category = "Interaction|Data")
	FText Name;

	/**
	 * @brief Whether a pawn can interact with this object
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bPawnInteract = true;

private:
	int InteractCount = 0;
};
