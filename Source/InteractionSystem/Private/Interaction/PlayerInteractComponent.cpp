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
	
	AActor* OwningActor = PrimitiveComponent->GetOwner();
	if (!OwningActor) { return nullptr; }

	return OwningActor->FindComponentByClass<UInteractableComponent>();
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

	// Invalidate currently viewed interactable if bPlayerInteract turns false
	if (HitInteractable && HoverPrimitive && !HitInteractable->CanInteract(GetOwner(), Hit.GetComponent()))
	{
		HoverInteractable = nullptr;
		HoverPrimitive = nullptr;
		OnUpdateInteract.Broadcast(false, nullptr);
		return;
	}
	
	/** Return early if there is no change */
	if (Hit.GetComponent() == HoverPrimitive) { return; }

	if (bHitInteractable && HitInteractable)
	{
		/** Set interact message when hovering over an interactable */
		if (HitInteractable->CanInteract(GetOwner(), Hit.GetComponent()))
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
	if (HoverInteractable)
	{
		/** Trigger interacted actor */
		HoverInteractable->Interact(GetOwner(), nullptr);
	}

	return HoverInteractable; 
}

void UPlayerInteractComponent::InteractAction()
{
	Interact();
}
