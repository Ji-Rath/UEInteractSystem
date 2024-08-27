// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickupable.generated.h"

class UProjectileMovementComponent;
class UItemInformation;

UCLASS()
class INTERACTIONSYSTEM_API APickupable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn), Category="Pickupable")
	UItemInformation* Item;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovement;
};
