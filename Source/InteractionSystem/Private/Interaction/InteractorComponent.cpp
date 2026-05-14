// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractorComponent.h"

#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/InteractableComponent.h"

DEFINE_LOG_CATEGORY(LogInteractor)

// Sets default values for this component's properties
UInteractorComponent::UInteractorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

// Called every frame
void UInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner()->HasLocalNetOwner())
	{
		UpdateHoverActor();
		
		if (!ActiveInteraction.IsExplicitlyNull() && ActiveInteraction != GetHoveredPrimitive())
		{
			FinishInteract();
		}
	}
}

void UInteractorComponent::UpdateHoverActor()
{
	/** Ensure that the owner is a pawn */
	APawn* Player = GetOwner<APawn>();
	if (!Player) return;
	APlayerController* PlayerController = Player->GetController<APlayerController>();
	if (!PlayerController) return;

	// Perform line trace
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
	FVector Distance = ForwardVector * InteractDistance;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(FName(TEXT("Interactor Trace")), false, Player);
	FHitResult Hit;
	FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();

	GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, CameraLocation + Distance, ECC_Visibility, QueryParams);
	
	// Check if hit is different from our current hover
	// This also checks if the actor suddenly disappears (like when being picked up)
	if (HoverPrimitive != Hit.Component || HoverPrimitive.IsStale())
	{
		OnUpdateHover.Broadcast(Hit.Component.Get());
	}

	HoverPrimitive = Hit.Component;
}

void UInteractorComponent::PerformInteraction(UPrimitiveComponent* Component)
{
	AActor* Owner = Component->GetOwner();
	if (auto Interactable = Owner->GetComponentByClass<UInteractableComponent>())
	{
		if (Interactable->IsPlayerInteractable())
		{
			ActiveInteraction = Component;
			Interactable->Interact(GetOwner(), Component);
		}
	}
}

void UInteractorComponent::PerformFinishInteraction()
{
	AActor* Owner = ActiveInteraction.IsValid() ? ActiveInteraction->GetOwner() : nullptr;
	UInteractableComponent* Interactable = Owner ? Owner->GetComponentByClass<UInteractableComponent>() : nullptr;
	
	if (Interactable)
	{
		Interactable->FinishInteract(GetOwner(), ActiveInteraction.Get());
	}
	
	ActiveInteraction.Reset();
}

void UInteractorComponent::ServerFinishInteract_Implementation()
{
	PerformFinishInteraction();
}

void UInteractorComponent::ServerInteract_Implementation(UPrimitiveComponent* Component)
{
	PerformInteraction(Component);
}

void UInteractorComponent::Interact()
{
	UE_LOG(LogInteractor, Verbose, TEXT("Attempting to interact with: %s"), *GetNameSafe(HoverPrimitive.Get()));
	if (HoverPrimitive.IsValid())
	{
		/** Trigger interacted actor */
		InteractWith(HoverPrimitive.Get());
		UE_LOG(LogInteractor, Log, TEXT("%s: Tried interacting with: %s - %s"), *GetNameSafe(this), *GetNameSafe(HoverPrimitive.Get()), *(HoverPrimitive.IsValid() ? GetNameSafe(HoverPrimitive->GetOwner()) : "NULL"));
	}
}

void UInteractorComponent::FinishInteract()
{
	if (!ActiveInteraction.IsValid()) { return; }
	
	PerformFinishInteraction();
	
	// Perform finish interaction on server as well
	if (!GetOwner()->HasAuthority())
	{
		ServerFinishInteract();
	}
}

void UInteractorComponent::InteractWith(UPrimitiveComponent* Component)
{
	PerformInteraction(Component);

	// Perform interaction on server as well
	if (!GetOwner()->HasAuthority())
	{
		ServerInteract(Component);
	}
}

void UInteractorComponent::InteractWith(AActor* Actor)
{
	InteractWith(Cast<UPrimitiveComponent>(Actor->GetRootComponent()));
}
