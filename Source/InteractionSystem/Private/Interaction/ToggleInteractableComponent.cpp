// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/ToggleInteractableComponent.h"


// Sets default values for this component's properties
UToggleInteractableComponent::UToggleInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UToggleInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	OnFinishInteract.AddDynamic(this, &UToggleInteractableComponent::ToggleState);
}


// Called every frame
void UToggleInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UToggleInteractableComponent::SetState(AActor* Interactor, bool bNewState)
{
	if (bIsOn != bNewState)
	{
		OnInteract.Broadcast(Interactor);
		bIsOn = bNewState;
	}
}

bool UToggleInteractableComponent::GetState() const
{
	return bIsOn;
}

void UToggleInteractableComponent::ToggleState()
{
	ToggleState(true);
}

void UToggleInteractableComponent::ToggleState(bool bShouldToggle)
{
	if (bShouldToggle)
	{
		bIsOn = !bIsOn;
	}
}

