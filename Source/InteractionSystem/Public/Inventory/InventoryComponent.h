// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInfo.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

struct FInstancedStruct;
class UItemInformation;
struct FItemHandle;
struct FInventoryContents;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryChange, const TArray<FInventoryContents>&, NewInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemAdd, const FInventoryContents&, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemRemove, const FInventoryContents&, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemChange, const FInventoryContents&, Item);

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);

/**
 * Inventory system that stores inventory values and allows manipulation of contents
 * Includes equip/unequip system
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Completely remove an item from current inventory */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	virtual void RemoveFromInventory(const FItemHandle& ItemHandle);

	/**
	 * Removes a specified item (and amount) from the inventory
	 * @param Item The item type and amount to remove
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	virtual void RemoveItemFromInventory(const FItemData& Item);

	/**
	 * Find the first slot containing Item
	 * @param ItemHandle - Item to find
	 * @return Slot with containing item
	*/
	UFUNCTION(BlueprintCallable, Category="Inventory")
	FInventoryContents GetItemByHandle(const FItemHandle& ItemHandle) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool FindItemByData(const UItemInformation* ItemData, FItemHandle& ItemHandle) const;

	/**
	 * Attempt to add an item to the inventory
	 * @param Item - Item to add
	 * @return Whether the item could be added to the inventory
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	virtual bool AddToInventory(const FItemData& Item, FItemHandle& OutItemHandle);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	virtual void EmptyInventory();

	//** Whether an item can be added to the inventory */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool CanAddToInventory(const FItemData& Item) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void GetInventory(TArray<FInventoryContents>& OutInventory) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	void SetInventory(const TArray<FInventoryContents>& NewInventory);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	FItemHandle GenerateUniqueHandle();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static FInventoryContents FindItemByHandle(const FItemHandle& ItemHandle);

	UFUNCTION(Category="Inventory")
	virtual FInventoryContents GenerateItem(const TInstancedStruct<FItemData>& ItemData);
	
protected:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Inventory, Category="Inventory")
	FInventoryContainer Inventory;
	
	UFUNCTION()
	virtual void OnRep_Inventory();
public:
	UPROPERTY(BlueprintAssignable)
	FInventoryChange OnInventoryChange;

	UPROPERTY(BlueprintAssignable)
	FItemAdd OnItemAdd;

	UPROPERTY(BlueprintAssignable)
	FItemChange OnItemChange;

	UPROPERTY(BlueprintAssignable)
	FItemRemove OnItemRemove;
};
