#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "InventoryInfo.h"
#include "PlayerEquipComponent.generated.h"

class USpringArmComponent;
class AInteractable;
class APickupable;
class UItemData;

UCLASS(Blueprintable, ClassGroup=(Interactable), meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UPlayerEquipComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPlayerEquipComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Equip the item that is in the slot */
	UFUNCTION(BlueprintCallable)
	void EquipItem(const FItemHandle& Item);

	UFUNCTION(BlueprintCallable)
	bool HasItemEquipped() const;

	/** Unequip any currently equipped items */
	UFUNCTION(BlueprintCallable)
	void UnequipItem();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerEquip")
	FItemHandle GetEquippedItem() const;

	UItemInformation* GetEquippedItemInfo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerEquip")
	AActor* GetEquippedActor() const;

	// Drop the currently equipped item
	UFUNCTION(BlueprintCallable, Category = "PlayerEquip")
	void DropEquippedItem();

	UFUNCTION(Server, Reliable)
	void ServerDropEquippedItem();

private:

	virtual void PerformDropEquippedItem();
	
	UPROPERTY(EditDefaultsOnly, Category = "PlayerEquip")
	FComponentReference ItemAttachParent;

	// The item that is currently equipped
	UPROPERTY(Replicated, SaveGame)
	FItemHandle EquippedItem;

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
	UInventoryComponent* InventoryComponent = nullptr;

	/** Called when there is an inventory update */
	UFUNCTION()
	void UpdateEquip(const TArray<FInventoryContents>& NewInventory);

	/** Called to use an item on an interactable */
	UFUNCTION(BlueprintCallable, Category = "PlayerEquip")
	void UseItem();

	UPROPERTY()
	FVector OriginalSocketOffset;

};

