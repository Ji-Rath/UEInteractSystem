
#include "Interaction/AreaInteractComponent.h"
#include "Components/ShapeComponent.h"
#include "Interaction/InteractableComponent.h"


void UAreaInteractComponent::HoverInteraction_Implementation(float DeltaTime)
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
		
		if (OverlappingPrimitives.Num() > 0)
		{
			/** Return early if there is no change */
			OverlappingPrimitives.Sort([this](const UPrimitiveComponent& Prim1, const UPrimitiveComponent& Prim2)
			{
				return FVector::Dist(Prim1.GetComponentLocation(), GetOwner()->GetActorLocation()) < FVector::Dist(Prim2.GetComponentLocation(), GetOwner()->GetActorLocation());
			});

			for(UPrimitiveComponent* PrimComp : OverlappingPrimitives)
			{
				if (HoverPrimitive == PrimComp) { return; }
			
				if (GetInteractComponent(PrimComp))
				{
					HoverPrimitive = PrimComp;
					HoverInteractable = GetInteractComponent(PrimComp);
					break;
				}
			}
		
			/** Set interact message when hovering over an interactable */
			if (HoverInteractable && HoverInteractable->bPlayerInteract)
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
