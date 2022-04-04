// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "Components/ActorComponent.h"
#include "Interaction/ItemData.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryChange, bool, bAdded);

USTRUCT(Blueprintable)
struct FInventoryContents
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle ItemData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Count;

	FInventoryContents()
	{
		ItemData = FDataTableRowHandle();
		Count = 0;
	}

	FInventoryContents(FDataTableRowHandle Item, int Amount)
	{
		ItemData = Item;
		Count = Amount;
	}

	bool operator==(const FInventoryContents& OtherSlot) const
	{
		return ItemData == OtherSlot.ItemData;
	}
};

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
	void DropItem(FDataTableRowHandle Item, const int Count = 1);

	/** Remove an item from current inventory */
	void RemoveFromInventory_Implementation(const FDataTableRowHandle Item, const int Count = 1);

	/**
	 * Find the first slot containing Item
	 * @param Item - Item to find
	 * @return Slot with containing item
	*/
	UFUNCTION(BlueprintCallable)
	int FindItemSlot(FDataTableRowHandle Item) const;

	/**
	 * Find the first slot containing Item
	 * @param Item - Item to find
	 * @return Slot with containing item
	*/
	UFUNCTION(BlueprintCallable)
	FDataTableRowHandle FindItem(const int Index) const;

	/**
	 * Attempt to add an item to the inventory
	 * @param Item - Item to add
	 * @param Count - Amount of item
	 * @return Whether the item could be added to the inventory
	*/
	bool AddToInventory_Implementation(FDataTableRowHandle Item, const int Count);

	UFUNCTION(BlueprintCallable)
	void GetInventory(TArray<FInventoryContents>& OutInventory) const;
	
private:
	UPROPERTY(EditAnywhere)
	TArray<FInventoryContents> Inventory;

	UPROPERTY(EditDefaultsOnly)
	int InventorySize;
};
