#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Interaction/InteractableComponent.h"
#include "PlayerEquipComponent.generated.h"

class USpringArmComponent;
class AInteractable;
class APickupable;
class UItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUseItem, const FInventoryContents&, ItemData);

UCLASS(Blueprintable, ClassGroup=(Interactable), meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UPlayerEquipComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;

public:
	UPlayerEquipComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Equip the item that is in the slot */
	UFUNCTION(BlueprintCallable)
	void EquipItem(const FInventoryContents& Item);

	/** Unequip any currently equipped items */
	UFUNCTION(BlueprintCallable)
	void UnequipItem();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerEquip")
	FInventoryContents GetEquippedItemData() const;

	FItemInfo GetEquippedItemInfo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerEquip")
	AActor* GetEquippedItem() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerEquip")
	void DropEquippedItem();

	UPROPERTY()
	FUseItem OnItemUse;
	
	UPROPERTY(BlueprintReadWrite, SaveGame)
	AActor* EquippedActor;

private:
	UPROPERTY(EditDefaultsOnly, Category = "PlayerEquip")
	FComponentReference ItemAttachParent;

	UPROPERTY()
	FInventoryContents EquippedItem;

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
	UInventoryComponent* InventoryCompRef = nullptr;

	TArray<FInventoryContents> Inventory;

	/** Called when there is an inventory update */
	UFUNCTION()
	void UpdateEquip(bool bAdded);

	/** Called to use an item on an interactable */
	UFUNCTION()
	void ItemInteract(UInteractableComponent* Interactable);

	UPROPERTY()
	FVector OriginalSocketOffset;

};

