// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/PickupableComponent.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemDataComponent.h"


// Sets default values for this component's properties
UPickupableComponent::UPickupableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ItemData.Count = 1;
	// ...
}


// Called when the game starts
void UPickupableComponent::BeginPlay()
{
	Super::BeginPlay();

	OnExecuteInteraction.AddDynamic(this, &UPickupableComponent::PickupItem);
}

void UPickupableComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimer);
}


// Called every frame
void UPickupableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPickupableComponent::PickupItem(AActor* Interactor)
{
	if (!ensure(Interactor)) { return; }
	
	UInventoryComponent* InventoryRef = Interactor->FindComponentByClass<UInventoryComponent>();

	/** Attempt to add the item to the inventory, destroy the item if successful */
	if (InventoryRef)
	{
		bool Success = IInventoryInterface::Execute_AddToInventory(InventoryRef, ItemData);

		// Delayed destruction is needed to handle async function calls when OnInteract is called
		FTimerDelegate DestroyDelegate;
		DestroyDelegate.BindLambda([&]
		{
			OnFinishInteract.Broadcast(true);
			GetOwner()->Destroy();
		});
		
		if (Success)
		{
			GetOwner()->SetActorEnableCollision(false);
			GetOwner()->SetActorHiddenInGame(true);
			AddInteractFilter(Interactor->GetClass(), false);
			GetWorld()->GetTimerManager().SetTimer(DestroyTimer, DestroyDelegate, 1.f, false);
		}
	}
}

FText UPickupableComponent::GetName() const
{
	if (FItemInfo* ItemInfo = ItemData.GetRow<FItemInfo>(""))
	{
		return ItemInfo->DisplayName;
	}
	return Name;
}

