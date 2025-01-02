// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickupable.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/InteractableComponent.h"

// Sets default values
APickupable::APickupable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
}

// Called when the game starts or when spawned
void APickupable::BeginPlay()
{
	Super::BeginPlay();
	
}

