
#include "Interaction/AreaInteractComponent.h"
#include "Components/ShapeComponent.h"
#include "Interaction/InteractableComponent.h"


void UAreaInteractComponent::HoverInteraction(float DeltaTime)
{
	/** Ensure that the owner is a pawn */
	APawn* Player = GetOwner<APawn>();
	if (!Player) return;
	APlayerController* PlayerController = Player->GetController<APlayerController>();
	if (!PlayerController) return;

	UShapeComponent* ShapeComp = Cast<UShapeComponent>(OverlapVolume.GetComponent(GetOwner()));
	if (ensure(ShapeComp))
	{
		TArray<UPrimitiveComponent*> OverlappingPrimitives;
		ShapeComp->GetOverlappingComponents(OverlappingPrimitives);

		HoverPrimitive = nullptr;
		if (OverlappingActors.Num() > 0)
		{
			/** Return early if there is no change */
			if (OverlappingPrimitives[0] == HoverPrimitive) { return; }
			HoverPrimitive = OverlappingPrimitives[0];
			HoverInteractable = GetInteractComponent(HoverPrimitive);
		
			/** Set interact message when hovering over an interactable */
			if (HoverInteractable && HoverInteractable->CanInteract(GetOwner()))
			{
				OnUpdateInteract.Broadcast(true, HoverInteractable);
				return;
			}
		}

		/** Send interaction update when there is no longer an interactable in view */
		if (HoverInteractable && HoverPrimitive == nullptr)
		{
			HoverInteractable = nullptr;
			OnUpdateInteract.Broadcast(false, nullptr);
		}
	}
}
