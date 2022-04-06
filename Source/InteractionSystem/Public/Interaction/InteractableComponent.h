// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteract, AActor*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FExecuteInteraction, AActor*, Interactor);
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

	UFUNCTION(BlueprintPure, BlueprintCallable, BlueprintNativeEvent)
	bool CanInteract(AActor* User);

	bool CanInteract(TSubclassOf<AActor> User);

	UFUNCTION(BlueprintCallable)
	void AddInteractFilter(TSubclassOf<AActor> User, bool bCanInteract);

	UFUNCTION(BlueprintCallable)
	void RemoveInteractFilter(TSubclassOf<AActor> User);

	UFUNCTION(BlueprintCallable)
	void ClearInteractFilter();

	UFUNCTION()
	void PerformInteractCheck(AActor* Interactor);

	/* Filters used to determine whether an actor can interact with this object */
	UPROPERTY(EditAnywhere, SaveGame, Category = "Interactable")
	TMap<TSubclassOf<AActor>, bool> InteractFilters;

	/* Called when the interactable component is interacted with */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FInteract OnInteract;

	/* Called when it is verified whether the interactor can interact with this actor (using InteractFilters) */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FExecuteInteraction OnExecuteInteraction;

	/* Optional delegate to help determine whether the interaction was successful */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FFinishInteract OnFinishInteract;

	/* Stores the prim component this interactable manages */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FComponentReference PrimComponent;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FText GetName() const;

	UPROPERTY(EditAnywhere)
	FText Name;

	/* Determines whether by default any object can interact with this actor */
	UPROPERTY(EditAnywhere)
	bool bDefaultInteractable = true;

	/* Determines whether a player even has the opportunity to interact with this object. This is useful when an interactable will only be used by other interactables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlayerInteract = true;

	/* Whether to display message when the interaction was successful. Relies on OnFinishInteract to be called */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable|Text")
	bool bInteractText = false;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bInteractText"), Category = "Interactable|Text")
	FText InteractMessage;

	/* Whether to display message when the interaction was unsuccessful. Relies on OnFinishInteract to be called */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable|Text")
	bool bCantInteractText = false;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bCantInteractText"), Category = "Interactable|Text")
	FText CantInteractMessage;

	/* Convenient function for broadcasting interact messages */
	UFUNCTION(BlueprintCallable)
	void SendInteractMessage(AActor* Interactor, FText Message);

	/* Convenience function that uses that last interactor to broadcast interact message */
	void SendInteractMessage(FText Message);

	/* The last actor that interacted with this object */
	UPROPERTY(BlueprintReadOnly)
	AActor* LastInteractor = nullptr;

	UFUNCTION()
	void SetLastInteractor(AActor* Interactor);

	UFUNCTION()
	void DisplayInteractMessage(bool bSuccess);
};
