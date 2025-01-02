// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractableComponent.h"

UInteractableComponent::UInteractableComponent(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
}

void UInteractableComponent::Interact(const AActor* Interactor, USceneComponent* Component)
{
	OnInteract.Broadcast(GetOwner(), Interactor, Component);
}

void UInteractableComponent::FinishInteract(const AActor* Interactor, USceneComponent* Component)
{
	OnFinishInteract.Broadcast(GetOwner(), Interactor, Component);
}

bool UInteractableComponent::IsPlayerInteractable() const
{
	return bIsPlayerInteractable;
}

EInteractType UInteractableComponent::GetInteractType() const
{
	return InteractType;
}
