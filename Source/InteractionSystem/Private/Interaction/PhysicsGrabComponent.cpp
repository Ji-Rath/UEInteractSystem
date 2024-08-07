#pragma once

#include "Interaction/PhysicsGrabComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UPhysicsGrabComponent::UPhysicsGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPhysicsGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPrimitiveComponent* GrabComp = HandleRef->GetGrabbedComponent();
	if (HandleRef && GrabComp && Player)
	{
		//CurrentGrabDistance = UKismetMathLibrary::FInterpTo(CurrentGrabDistance, GrabRange, DeltaTime, 3.f);
		const APlayerController* PlayerController = Player->GetController<APlayerController>();
		const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
		const FVector Distance = ForwardVector * CurrentGrabDistance;
		const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
		const FVector TargetLocation = CameraLocation + Distance;
		
		/** Update the target location to be in front of the players camera */
		HandleRef->SetTargetLocation(TargetLocation);

		/** Release grabbed component if it goes out of range */
		const bool bOutsideRange = FVector::Distance(TargetLocation, GrabComp->GetComponentLocation()) > MaxDistance;
		if (bOutsideRange)
		{
			ReleaseComponent();
		}
	}
}

void UPhysicsGrabComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Player = GetOwner<APawn>();
	check(Player);
	
	HandleRef = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	ensure(HandleRef);

	const APlayerController* PlayerController = Player->GetController<APlayerController>();
	if  (PlayerController)
	{
		InputScale.Pitch = PlayerController->InputPitchScale_DEPRECATED;
		InputScale.Roll = PlayerController->InputRollScale_DEPRECATED;
		InputScale.Yaw = PlayerController->InputYawScale_DEPRECATED;
	}
}

void UPhysicsGrabComponent::ReleaseComponent()
{
	if (Player && HandleRef)
	{
		UPrimitiveComponent* GrabbedComponent = HandleRef->GetGrabbedComponent();
		APlayerController* PlayerController = Player->GetController<APlayerController>();
		if (GrabbedComponent && PlayerController)
		{
			GrabbedComponent->SetCollisionResponseToChannel(ECC_Pawn, PawnResponse);
			HandleRef->ReleaseComponent();
		}
	}
}

void UPhysicsGrabComponent::GrabComponent(UStaticMeshComponent* GrabMesh, FHitResult Hit)
{
	if (Player)
	{
		APlayerController* PlayerController = Player->GetController<APlayerController>();
		if (PlayerController)
		{
			const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
			HandleRef->GrabComponentAtLocationWithRotation(GrabMesh, Hit.BoneName, Hit.ImpactPoint, Hit.GetComponent()->GetComponentRotation());
			HandleRef->SetTargetLocation(Hit.ImpactPoint);
			PawnResponse = GrabMesh->GetCollisionResponseToChannel(ECC_Pawn);
			GrabMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			CurrentGrabDistance = FVector::Distance(Hit.ImpactPoint, CameraLocation);
		}
	}
}

void UPhysicsGrabComponent::PushComponent(UStaticMeshComponent* GrabMesh)
{
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
	if (bCanPush && !GetWorld()->GetTimerManager().IsTimerActive(PushTimer))
	{
		GrabMesh->AddImpulse(ForwardVector * PushImpulse);
		GetWorld()->GetTimerManager().SetTimer(PushTimer, PushDelay, false);
	}
}

void UPhysicsGrabComponent::PushGrabbedComponent()
{
	UStaticMeshComponent* PushMesh = Cast<UStaticMeshComponent>(HandleRef->GetGrabbedComponent());
	if (PushMesh)
	{
		ReleaseComponent();
		PushComponent(PushMesh);
	}
}

void UPhysicsGrabComponent::PhysicsInteract()
{
	/** Ensure that the pawn owner is valid */
	if (!Player) return;
	const APlayerController* PlayerController = Player->GetController<APlayerController>();
	
	UStaticMeshComponent* GrabMesh = Cast<UStaticMeshComponent>(HandleRef->GetGrabbedComponent());
	if (!GrabMesh)
	{
		const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
		const FVector Distance = ForwardVector * InteractDistance;
		const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();

		FHitResult Hit;
		const FCollisionObjectQueryParams QueryParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody);
		const bool bTraced = GetWorld()->LineTraceSingleByObjectType(OUT Hit, CameraLocation, CameraLocation + Distance, QueryParams);

		/** Ensure there was a trace and that we are not dragging something triggerable */
		GrabMesh = Cast<UStaticMeshComponent>(Hit.GetComponent());
		if (bTraced && GrabMesh)
		{
			/** Grab object if below threshhold or simply push it */
			if (bCanGrab && GrabMesh->GetMass() < GrabWeightThreshold)
			{
				GrabComponent(GrabMesh, Hit);
			}
		}
	}
	else
	{
		ReleaseComponent();
	}
}
