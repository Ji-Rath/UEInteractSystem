// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractableComponent.h"

#include "GameFramework/Actor.h"
#include "Interaction/PlayerInteractComponent.h"


// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

bool UInteractableComponent::CanInteract_Implementation(AActor* Interactor, UPrimitiveComponent* Component)
{
	return true;
}

void UInteractableComponent::Interact(AActor* Interactor, UPrimitiveComponent* Component)
{
	if (CanInteract(Interactor, Component))
	{
		PerformInteraction(Interactor, Component);

		// If we dont have authority, also trigger call on server
		if (!GetOwner()->HasAuthority())
		{
			ServerInteract(Interactor, Component);
		}
	}
}

void UInteractableComponent::ServerInteract_Implementation(AActor* Interactor, UPrimitiveComponent* Component)
{
	if (CanInteract(Interactor, Component))
	{
		PerformInteraction(Interactor, Component);
	}
}

void UInteractableComponent::PerformInteraction(AActor* Interactor, UPrimitiveComponent* Component)
{
	OnInteract.Broadcast(Interactor, Component);
}

FText UInteractableComponent::GetName_Implementation(UPrimitiveComponent* Component) const
{
	return DefaultName;
}

void UInteractableComponent::SendInteractMessage(AActor* Interactor, FText Message)
{
	if (!ensureMsgf(Interactor, TEXT("SendInteractMessage requires a valid Interactor!")))
	{
		return;
	}
	
	if (auto* PlayerInteract = Interactor->FindComponentByClass<UPlayerInteractComponent>())
	{
		PlayerInteract->OnSendInteractMessage.Broadcast(Message);
	}
}


