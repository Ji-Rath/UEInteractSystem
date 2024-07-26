
#include "Interaction/AreaInteractComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UAreaInteractComponent::UpdateHoverActor()
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

		auto ClosestPrimitive = OverlappingPrimitives.IsEmpty() ? nullptr : OverlappingPrimitives[0];

		if (ClosestPrimitive != HoverPrimitive)
		{
			OnUpdateHover.Broadcast(ClosestPrimitive);
		}
		HoverPrimitive = ClosestPrimitive;
	}
}
