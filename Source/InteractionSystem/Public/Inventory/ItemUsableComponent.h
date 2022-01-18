// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemUsableComponent.generated.h"

class UItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemUse, AActor*, User, UItemData*, ItemData);

UCLASS(Blueprintable, ClassGroup=("Interactable|Inventory"), meta=(BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UItemUsableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UItemUsableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable, BlueprintCallable)
	FItemUse OnItemUse;
};
