﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/PickupableComponent.h"

#include "GameFramework/Actor.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemDataComponent.h"


// Sets default values for this component's properties
UPickupableComponent::UPickupableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPickupableComponent::BeginPlay()
{
	Super::BeginPlay();

	OnInteract.AddDynamic(this, &UPickupableComponent::PickupItem);
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
	UItemDataComponent* ItemData = GetOwner()->FindComponentByClass<UItemDataComponent>();

	/** Attempt to add the item to the inventory, destroy the item if successful */
	if (InventoryRef && ensureMsgf(ItemData, TEXT("Cannot add item to inventory without ItemData!")))
	{
		bool Success = InventoryRef->AddToInventory(ItemData->GetItemData(), Amount);
		if (Success)
			GetOwner()->Destroy();
	}
}

