// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractableComponent.h"

#include "GameFramework/Actor.h"
#include "Interaction/PlayerInteractComponent.h"


// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	// Update last interactor when the interactable is interacted with
	OnInteract.AddDynamic(this, &UInteractableComponent::SetLastInteractor);
	OnInteract.AddDynamic(this, &UInteractableComponent::PerformInteractCheck);
	OnFinishInteract.AddDynamic(this, &UInteractableComponent::DisplayInteractMessage);
}


// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractableComponent::DisplayInteractMessage(bool bSuccess)
{
	FText Message = bSuccess ? InteractMessage : CantInteractMessage;
	SendInteractMessage(Message);
}

bool UInteractableComponent::CanInteract(TSubclassOf<AActor> User)
{
	for (auto& Elem : InteractFilters)
	{
		if (User->StaticClass()->IsChildOf(Elem.Key->StaticClass()))
		{
			return Elem.Value;
		}
	}
	return bDefaultInteractable;
}

void UInteractableComponent::AddInteractFilter(TSubclassOf<AActor> User, bool bCanInteract)
{
	InteractFilters.Add(User, bCanInteract);
}

void UInteractableComponent::RemoveInteractFilter(TSubclassOf<AActor> User)
{
	InteractFilters.Remove(User);
}

void UInteractableComponent::ClearInteractFilter()
{
	InteractFilters.Empty();
}

void UInteractableComponent::PerformInteractCheck(AActor* Interactor)
{
	bool bCanInteract = CanInteract(Interactor);
	if (bCanInteract)
	{
		OnExecuteInteraction.Broadcast(Interactor);
	}
	
	OnFinishInteract.Broadcast(bCanInteract);
}

bool UInteractableComponent::CanInteract_Implementation(AActor* User)
{
	return CanInteract(User->StaticClass());
}

FText UInteractableComponent::GetName() const
{
	FText DisplayName = Name;
	
	return DisplayName;
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

void UInteractableComponent::SendInteractMessage(FText Message)
{
	SendInteractMessage(LastInteractor, Message);
}

void UInteractableComponent::SetLastInteractor(AActor* Interactor)
{
	LastInteractor = Interactor;
}


