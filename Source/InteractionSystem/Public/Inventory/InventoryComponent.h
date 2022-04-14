// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "Components/ActorComponent.h"
#include "Interaction/ItemData.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryChange, bool, bAdded);

/**
 * Inventory system that stores inventory values and allows manipulation of contents
 * Includes equip/unequip system
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UInventoryComponent : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FInventoryChange OnInventoryChange;

	/** Drop an item from selected slot */
	UFUNCTION(BlueprintCallable)
	void DropItem(const FInventoryContents& Item);

	/** Remove an item from current inventory */
	virtual void RemoveFromInventory_Implementation(const FInventoryContents& Item) override;

	/**
	 * Find the first slot containing Item
	 * @param Item - Item to find
	 * @return Slot with containing item
	*/
	UFUNCTION(BlueprintCallable)
	int FindItemSlot(const FInventoryContents& Item) const;

	/**
	 * Find the first slot containing Item
	 * @param Item - Item to find
	 * @return Slot with containing item
	*/
	UFUNCTION(BlueprintCallable)
	FInventoryContents FindItem(const int Index) const;

	/**
	 * Attempt to add an item to the inventory
	 * @param Item - Item to add
	 * @param Count - Amount of item
	 * @return Whether the item could be added to the inventory
	*/
	virtual bool AddToInventory_Implementation(const FInventoryContents& Item) override;

	UFUNCTION(BlueprintCallable)
	void GetInventory(TArray<FInventoryContents>& OutInventory) const;

	void SetInventory(const TArray<FInventoryContents>& NewInventory);

	// Base class of pickupable when no custom class is given
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ItemBaseClass;

private:
	UPROPERTY(EditAnywhere)
	TArray<FInventoryContents> Inventory;

	UPROPERTY(EditDefaultsOnly)
	int InventorySize;

	UPROPERTY(EditAnywhere)
	UDataTable* FallbackTable = nullptr;
};
