// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/TriggerComponent.h"

#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractableComponent.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
}

void UTriggerComponent::TriggerActors(AActor* Instigator, UPrimitiveComponent* Component)
{
	FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &UTriggerComponent::ExecuteInteraction, Instigator);

	bool bCanTrigger = true;
	if (bCanTrigger)
	{
		if (TriggerDelay > 0.f)
		{
			/** Only apply timer if its not already set */
			if (!GetOwner()->GetWorldTimerManager().IsTimerActive(DelayHandle))
				GetOwner()->GetWorldTimerManager().SetTimer(DelayHandle, TimerDel, TriggerDelay, false);
		}
		else
		{
			/** Trigger actors *instantly* if there is no delay set */
			if (!GetOwner()->GetWorldTimerManager().IsTimerActive(DelayHandle))
				GetOwner()->GetWorldTimerManager().SetTimer(DelayHandle, TimerDel, 0.1f, false);
		}

		TriggerCount++;
	}
}

void UTriggerComponent::ExecuteInteraction(AActor* Instigator)
{
	/** Call trigger function for all actors in array */
	for (const auto Interactable : Interactables)
	{
		if (auto InteractableComponent = Cast<UInteractableComponent>(Interactable.GetComponent(Interactable.OtherActor.Get())))
		{
			InteractableComponent->Interact(Instigator, Interactable.OtherActor->GetRootComponent());
		}
	}
}

