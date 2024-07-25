// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/ToggleInteractableComponent.h"

#include "Net/UnrealNetwork.h"


void UToggleInteractableComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UToggleInteractableComponent, bIsOn);
}

// Sets default values for this component's properties
UToggleInteractableComponent::UToggleInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UToggleInteractableComponent::SetState(bool bNewState)
{
	SetState(GetOwner(), bNewState);
}

void UToggleInteractableComponent::SetState(AActor* Interactor, bool bNewState)
{
	if (bIsOn != bNewState)
	{
		Interact(Interactor);
	}
}

bool UToggleInteractableComponent::GetState()
{
	return bIsOn;
}

void UToggleInteractableComponent::ToggleState(AActor* Interactor)
{
	bIsOn = !bIsOn;
}

void UToggleInteractableComponent::PerformInteraction(AActor* Interactor, UPrimitiveComponent* Component)
{
	if (CanInteract(Interactor, Component))
	{
		ToggleState(Interactor);
	}
	
	Super::PerformInteraction(Interactor, Component);
}

void UToggleInteractableComponent::ToggleState()
{
	bIsOn = !bIsOn;
}

