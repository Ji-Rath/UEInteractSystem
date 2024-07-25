// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "InventoryInfo.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Interaction/InteractableComponent.h"
#include "PickupableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUseItem, AActor*, User);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UPickupableComponent : public UInteractableComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPickupableComponent();

	/** Add item to interactors inventory */
	UFUNCTION(BlueprintCallable)
	void PickupItem(AActor* Interactor);
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FUseItem OnUseItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickupable")
	FInventoryContents ItemData;
	
	FTimerHandle DestroyTimer;

	virtual FText GetName_Implementation(UPrimitiveComponent* Component) const override;

	virtual void PerformInteraction(AActor* Interactor, UPrimitiveComponent* Component) override;

	UFUNCTION(BlueprintCallable, Category = "Pickupable")
	virtual void UseItem(AActor* User);
};
