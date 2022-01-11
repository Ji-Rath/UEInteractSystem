#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsGrabComponent.generated.h"

class IInteraction;
class UPhysicsHandleComponent;
class AInteractable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGrabUpdate, bool, bGrab, AInteractable*, Actor);

/**
 * Allows a pawn to grab a physics body through the use of a physics handle component
 */
UCLASS(Blueprintable, ClassGroup = (Interactable), meta = (BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UPhysicsGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPhysicsGrabComponent();

	virtual void BeginPlay() override;

	/** Release the currently grabbed component */
	UFUNCTION(BlueprintCallable)
	void ReleaseComponent();

	/** Grab the selected mesh */
	UFUNCTION(BlueprintCallable)
	void GrabComponent(UStaticMeshComponent* GrabMesh, FHitResult Hit);

	/** Push the selected mesh */
	UFUNCTION(BlueprintCallable)
	void PushComponent(UStaticMeshComponent* GrabMesh);
	
	void PushComponent();

	/** Called to grab the in-range physics object */
	void PhysicsInteract();

	UPROPERTY()
	FGrabUpdate OnGrabUpdate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics|Grab")
	bool bCanGrab = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics|Push")
	bool bCanPush = true;

private:
	UPROPERTY()
	UPhysicsHandleComponent* HandleRef = nullptr;

	UPROPERTY()
	APawn* Player = nullptr;

	UPROPERTY(EditAnywhere, Category = "Physics")
	float InteractDistance = 100.f;

	UPROPERTY(EditAnywhere, Category = "Physics|Grab")
	float GrabRange = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Physics|Grab")
	float GrabWeightThreshold = 25.f;

	UPROPERTY(EditDefaultsOnly, Category = "Physics|Push")
	float PushImpulse = 100000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Physics|Grab")
	float MaxDistance = 100.f;

	float CurrentGrabDistance = 0.f;

	FTimerHandle PushTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Physics|Push")
	float PushDelay = 1.f;
private:
	FRotator InputScale;

	ECollisionResponse PawnResponse;
};


