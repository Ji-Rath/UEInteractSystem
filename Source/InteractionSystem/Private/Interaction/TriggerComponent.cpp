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

void UTriggerComponent::TriggerActors(AActor* Instigator)
{
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &UTriggerComponent::ExecuteInteraction, Instigator);

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
	for (FComponentReference Interactable : InteractablesToTrigger)
	{
		auto* InteractableTrigger = Cast<UInteractableComponent>(Interactable.GetComponent(Interactable.OtherActor.Get()));
		auto* ToggleInteractableTrigger = Cast<UToggleInteractableComponent>(Interactable.GetComponent(Interactable.OtherActor.Get()));
		auto* ToggleInteractable = GetOwner()->FindComponentByClass<UToggleInteractableComponent>();

		if (InteractableTrigger && bModifyFilters)
		{
			InteractableTrigger->InteractFilters.Append(ModifyFilters);
		}

		if (bCallOnInteract)
		{
			// For toggle interactables, match the state so they are synced together
			if (ToggleInteractable && ToggleInteractableTrigger)
			{
				// Since the state of the interactable changes before triggering other actors, the condition has to be inverted
				if (ToggleInteractable->GetState() != ToggleInteractableTrigger->GetState())
					InteractableTrigger->OnInteract.Broadcast(Instigator);
			}
			else
			{
				InteractableTrigger->OnInteract.Broadcast(Instigator);
			}
		}
	}
}

