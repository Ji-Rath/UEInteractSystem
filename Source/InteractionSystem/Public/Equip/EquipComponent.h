// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventoryInfo.h"
#include "EquipComponent.generated.h"

class UItemAction;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateEquipState, const FItemHandle&, EquippedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateUseState, const FItemHandle&, EquippedItem, bool, bUsingItem);

UCLASS(Blueprintable, ClassGroup=(Interactable), meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEquipComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void ItemAdded(const FInventoryContents& Item);
	
	/** Called when there is an inventory update */
	UFUNCTION()
	void UpdateEquip(const TArray<FInventoryContents>& NewInventory);
	
	// The item that is currently equipped
	UPROPERTY(Replicated, SaveGame)
	FItemHandle EquippedItem;
	
	UPROPERTY()
	UInventoryComponent* InventoryComponent = nullptr;
	
	// The currently used action
	UPROPERTY()
	UItemAction* CurrentAction = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerEquip")
	bool bUsingItem = false;

public:
	/** Equip the item that is in the slot */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PlayerEquip")
	void EquipItem(const FItemHandle& Item);
	virtual void EquipItem_Implementation(const FItemHandle& Item);
	
	/** Unequip any currently equipped items */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PlayerEquip")
	void UnequipItem();
	virtual void UnequipItem_Implementation();
	
	/** Equips the specified item. Will unequip if the item is already equipped */
	UFUNCTION(BlueprintCallable, Category = "PlayerEquip")
	void ToggleEquippedItem(const FItemHandle& Item);
	
	UFUNCTION(BlueprintCallable, Category = "PlayerEquip")
	UItemAction* GetItemAction() const;

	/**
	 * Determines whether the player has an item equipped
	 * @return Whether we have an item equipped
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayerEquip")
	bool HasItemEquipped() const;
	
	UFUNCTION(BlueprintCallable, Category = "PlayerEquip")
	bool IsUsingItem() const;

	/**
	 * Returns a handle to the equipped item if valid
	 * @return The currently equipped item
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerEquip")
	FItemHandle GetEquippedItem() const;

	/**
	 * Returns item information related to the equipped item
	 * @return ItemInformation pointer to the equipped item
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerEquip")
	UItemInformation* GetEquippedItemInfo() const;
	
	/** Called to use an item on an interactable */
	UFUNCTION(BlueprintCallable, Category = "PlayerEquip")
	virtual void UseItem();

	UFUNCTION(BlueprintCallable, Category = "PlayerEquip")
	virtual void FinishUseItem();
	
	// Triggers whenever the player's equip state changes
	// @note This delegate will get called after the item "Visual" bundle has been loaded
	UPROPERTY(BlueprintAssignable, Category = "PlayerEquip")
	FUpdateEquipState OnUpdateEquipState;
	
	// Triggers whenever the player starts/finishes using an item
	UPROPERTY(BlueprintAssignable, Category = "PlayerEquip")
	FUpdateUseState OnUpdateUseState;
	
	/* Determines whether we automatically equip items we pick up */
	UPROPERTY(EditDefaultsOnly, Category = "PlayerEquip")
	bool bAutoEquip = true;
};
