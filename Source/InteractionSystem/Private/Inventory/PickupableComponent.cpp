// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/PickupableComponent.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Interaction/ItemData.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryInfo.h"


// Sets default values for this component's properties
UPickupableComponent::UPickupableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

void UPickupableComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimer);
}

void UPickupableComponent::PickupItem(AActor* Interactor)
{
	if (!ensure(Interactor)) { return; }
	
	UInventoryComponent* InventoryRef = Interactor->FindComponentByClass<UInventoryComponent>();

	/** Attempt to add the item to the inventory, destroy the item if successful */
	if (InventoryRef)
	{
		bool Success = true;

		// Delayed destruction is needed to handle async function calls when OnInteract is called
		FTimerDelegate DestroyDelegate;
		DestroyDelegate.BindLambda([&]
		{
			GetOwner()->Destroy();
		});
		
		if (Success)
		{
			GetOwner()->SetActorEnableCollision(false);
			GetOwner()->SetActorHiddenInGame(true);
			GetWorld()->GetTimerManager().SetTimer(DestroyTimer, DestroyDelegate, 1.f, false);
		}
	}
}

FText UPickupableComponent::GetName_Implementation(UPrimitiveComponent* Component) const
{
	if (auto* ItemInfo = ItemData.ItemInformation)
	{
		return ItemInfo->DisplayName;
	}
	return Super::GetName(Component);
}

void UPickupableComponent::PerformInteraction(AActor* Interactor, UPrimitiveComponent* Component)
{
	PickupItem(Interactor);
	Super::PerformInteraction(Interactor, Component);
}

