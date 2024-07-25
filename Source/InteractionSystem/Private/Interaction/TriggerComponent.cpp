// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/TriggerComponent.h"

#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Interaction/ToggleInteractableComponent.h"

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

	if (bAutoManage)
	{
		if (UInteractableComponent* Owner = GetOwner()->FindComponentByClass<UInteractableComponent>())
		{
			Owner->OnInteract.AddDynamic(this, &UTriggerComponent::TriggerActors);
		}	
	}
}

void UTriggerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (UInteractableComponent* Owner = GetOwner()->FindComponentByClass<UInteractableComponent>())
	{
		Owner->OnInteract.RemoveDynamic(this, &UTriggerComponent::TriggerActors);
	}
}

void UTriggerComponent::TriggerActors(AActor* Instigator, UPrimitiveComponent* Component)
{
	FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &UTriggerComponent::ExecuteInteraction, Instigator);

	bool bCanTrigger = TriggerCount < TriggerAmount || TriggerAmount == 0;
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
	for (const auto Interactable : InteractablesToTrigger)
	{
		if (!Interactable) { continue; }
		auto* InteractableTrigger = Interactable->FindComponentByClass<UInteractableComponent>();
		auto* ToggleInteractableTrigger = Interactable->FindComponentByClass<UToggleInteractableComponent>();
		auto* ToggleInteractable = GetOwner()->FindComponentByClass<UToggleInteractableComponent>();

		if (bCallOnInteract)
		{
			// For toggle interactables, match the state so they are synced together
			if (ToggleInteractable && ToggleInteractableTrigger)
			{
				// Since the state of the interactable changes before triggering other actors, the condition has to be inverted
				if (ToggleInteractable->GetState() != ToggleInteractableTrigger->GetState())
					InteractableTrigger->Interact(Instigator);
			}
			else
			{
				InteractableTrigger->Interact(Instigator);
			}
		}
	}
}

