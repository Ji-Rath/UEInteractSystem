#pragma once

#include "CoreMinimal.h"
#include "Equip/EquipComponent.h"
#include "Item/ItemVisual.h"
#include "PlayerEquipComponent.generated.h"

class USpringArmComponent;
class AInteractable;
class APickupable;
class UItemData;

UCLASS(Blueprintable, ClassGroup=(Interactable), meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UPlayerEquipComponent : public UEquipComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPlayerEquipComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EquipItem_Implementation(const FItemHandle& Item) override;

private:

	virtual void PerformDropEquippedItem();
	
	// Drop the currently equipped item
	UFUNCTION(BlueprintCallable, Category = "PlayerEquip")
	void DropEquippedItem();

	UFUNCTION(Server, Reliable)
	void ServerDropEquippedItem();
	
	UPROPERTY(EditDefaultsOnly, Category = "PlayerEquip")
	FComponentReference ItemAttachParent;

	virtual void UnequipItem_Implementation() override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerEquip")
	AActor* GetEquippedActor() const;
	
	UFUNCTION()
	void EquipActorSpawned(AItemVisual* Actor);

	// Visual representation of the equipped item
	UPROPERTY()
	AActor* EquippedActor;

	/** Starting offset for equipping an item */
	UPROPERTY(EditDefaultsOnly, Category = "PlayerEquip")
	float ItemUnequipOffset = -25.f;

	/** Offset when the equipped item is fully on screen */
	UPROPERTY(EditDefaultsOnly, Category = "PlayerEquip")
	float InitialSpringArmOffset = 0.f;

	/** Speed to bring equipped item into view */
	UPROPERTY(EditDefaultsOnly, Category = "PlayerEquip")
	float EquipInterpSpeed = 3.f;

	/** Impulse to throw equipped item */
	UPROPERTY(EditDefaultsOnly, Category = "PlayerEquip")
	float ThrowImpulse = 500.f;

	UPROPERTY()
	USpringArmComponent* ItemAttachSpring = nullptr;

	UPROPERTY()
	FVector OriginalSocketOffset;

};

