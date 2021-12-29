// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Interactable.h"
#include "Interaction/ItemData.h"
#include "Interaction/TriggerComponent.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	OnInteracted.AddDynamic(this, &AInteractable::OnInteract);
}

void AInteractable::OnInteract_Implementation(AActor* Interactor)
{
	//Blank implementation
}

bool AInteractable::Interact(AActor* Interactor)
{
	const bool bReachedInteractLimit = !(InteractAmount == -1 || (InteractCount < InteractAmount));
	if (CanInteract(Interactor) && !bReachedInteractLimit)
	{
		InteractCount++;
		OnInteracted.Broadcast(Interactor);

		return true;
	}
	return false;
}

bool AInteractable::CanInteract_Implementation(AActor* Interactor) const
{
	bool bSuccess = true;
	
	if (!bPawnInteract && Cast<APawn>(Interactor))
	{
		bSuccess = false;
	}
	return bSuccess;
}

FText AInteractable::GetName() const
{
	return Name;
}

