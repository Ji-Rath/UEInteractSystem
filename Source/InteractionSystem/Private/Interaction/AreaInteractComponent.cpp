
#include "Interaction/AreaInteractComponent.h"
#include "Components/ShapeComponent.h"
#include "Interaction/InteractableComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UAreaInteractComponent::HoverInteraction_Implementation(float DeltaTime)
{
	/** Ensure that the owner is a pawn */
	APawn* Player = GetOwner<APawn>();
	if (!Player) return;
	APlayerController* PlayerController = Player->GetController<APlayerController>();
	if (!PlayerController) return;
	
	TArray<UPrimitiveComponent*> OverlappingPrimitives;
	UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), GetOwner()->GetActorLocation(), InteractDistance, {}, nullptr, {GetOwner()}, OverlappingPrimitives);
	UPrimitiveComponent* SelectedInteractable = nullptr;
	
	if (OverlappingPrimitives.Num() > 0)
	{
		/** Return early if there is no change */
		OverlappingPrimitives.Sort([this](const UPrimitiveComponent& Prim1, const UPrimitiveComponent& Prim2)
		{
			return FVector::DistSquared(Prim1.GetComponentLocation(), GetOwner()->GetActorLocation()) < FVector::DistSquared(Prim2.GetComponentLocation(), GetOwner()->GetActorLocation());
		});

		for(UPrimitiveComponent* PrimComp : OverlappingPrimitives)
		{
			if (HoverPrimitive == PrimComp) { return; }
		
			if (GetInteractComponent(PrimComp))
			{
				HoverPrimitive = PrimComp;
				HoverInteractable = GetInteractComponent(PrimComp);
				SelectedInteractable = PrimComp;
				break;
			}
		}
	}

	/** Send interaction update when there is no longer an interactable in view */
	if (HoverInteractable)
	{
		/** Set interact message when hovering over an interactable */
		if (SelectedInteractable == nullptr)
		{
			HoverInteractable = nullptr;
			HoverPrimitive = nullptr;
			OnUpdateInteract.Broadcast(false, nullptr);
		}
		else if (HoverInteractable->CanInteract(GetOwner(), nullptr))
		{
			OnUpdateInteract.Broadcast(true, HoverInteractable);
		} 
	}
}
