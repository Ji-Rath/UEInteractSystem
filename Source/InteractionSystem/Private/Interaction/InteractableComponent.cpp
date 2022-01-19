// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractableComponent.h"

#include "GameFramework/Actor.h"
#include "Interaction/ItemData.h"
#include "Inventory/ItemDataComponent.h"


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

	// ...
	
}


// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractableComponent::SetCanInteract(bool bCanInteract)
{
	bInteractable = bCanInteract;
}

bool UInteractableComponent::CanInteract_Implementation()
{
	return bInteractable;
}

FText UInteractableComponent::GetName() const
{
	FText DisplayName = Name;
	
	// Replace name with itemdata name if component exists
	if (DisplayName.IsEmpty())
	{
		if (const UItemDataComponent* ItemDataComponent = GetOwner()->FindComponentByClass<UItemDataComponent>())
		{
			if (const UItemData* ItemData = ItemDataComponent->GetItemData())
				DisplayName = ItemData->Name;
		}
	}
	
	return DisplayName;
}

FText UInteractableComponent::GetInteractText()
{
	return InteractText;
}

void UInteractableComponent::SetInteractText(FText NewText)
{
	InteractText = NewText;
}

