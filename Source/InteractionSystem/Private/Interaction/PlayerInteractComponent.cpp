// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/PlayerInteractComponent.h"

#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/InteractableComponent.h"


// Sets default values for this component's properties
UPlayerInteractComponent::UPlayerInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called every frame
void UPlayerInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HoverInteraction(DeltaTime);
}

UInteractableComponent* UPlayerInteractComponent::GetInteractComponent(UPrimitiveComponent* PrimitiveComponent)
{
	if (!PrimitiveComponent) { return nullptr; }
	
	UInteractableComponent* InteractableComponent = nullptr;
	
	AActor* OwningActor = PrimitiveComponent->GetOwner();
	if (!OwningActor) { return nullptr; }
	
	TInlineComponentArray<UInteractableComponent*> InteractableComponents(OwningActor);
	OwningActor->GetComponents(InteractableComponents);

	// Loop through InteractableComponents and find the matching component
	for(UInteractableComponent* Interactable : InteractableComponents)
	{
		if (Interactable->PrimComponent.GetComponent(OwningActor) == PrimitiveComponent)
		{
			InteractableComponent = Interactable;
			break;
		}
	}

	return InteractableComponent;
}

void UPlayerInteractComponent::HoverInteraction_Implementation(float DeltaTime)
{
	/** Ensure that the owner is a pawn */
	APawn* Player = GetOwner<APawn>();
	if (!Player) return;
	APlayerController* PlayerController = Player->GetController<APlayerController>();
	if (!PlayerController) return;

	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
	FVector Distance = ForwardVector * InteractDistance;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(FName(TEXT("Interaction Actor")), false, Player);
	FHitResult Hit;
	FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();

	bool bHitInteractable = GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, CameraLocation + Distance, ECC_Visibility, QueryParams);
	UInteractableComponent* HitInteractable = GetInteractComponent(Hit.GetComponent());

	/** Return early if there is no change */
	if (Hit.GetComponent() == HoverPrimitive) { return; }

	if (bHitInteractable && HitInteractable)
	{
		/** Set interact message when hovering over an interactable */
		if (HitInteractable->bPlayerInteract)
		{
			HoverInteractable = HitInteractable;
			HoverPrimitive = Hit.GetComponent();
			OnUpdateInteract.Broadcast(true, HoverInteractable);
			return;
		}
	}
	
	
	/** Send interaction update when there is no longer an interactable in view */
	if (HoverInteractable && !HitInteractable)
	{
		HoverInteractable = nullptr;
		HoverPrimitive = nullptr;
		OnUpdateInteract.Broadcast(false, nullptr);
	}
}

UInteractableComponent* UPlayerInteractComponent::Interact()
{
	// Allows classes to use OnInteract without player looking at interactable (ex. using equipped item)
	OnInteract.Broadcast(HoverInteractable);

	if (HoverInteractable)
	{
		/** Trigger interacted actor */
		HoverInteractable->OnInteract.Broadcast(GetOwner());
	}

	return HoverInteractable; 
}

void UPlayerInteractComponent::InteractAction()
{
	Interact();
}
