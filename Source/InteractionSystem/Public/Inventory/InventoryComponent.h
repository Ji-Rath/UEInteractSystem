// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

struct FInstancedStruct;
class UItemInformation;
struct FItemHandle;
struct FInventoryContents;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryChange, const TArray<FInventoryContents>&, NewInventory);
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
	/** Remove an item from current inventory */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	virtual void RemoveFromInventory(const FItemHandle& ItemHandle);

	/**
	 * Find the first slot containing Item
	 * @param ItemHandle - Item to find
	 * @return Slot with containing item
	*/
	UFUNCTION(BlueprintCallable, Category="Inventory")
	FInventoryContents GetItemByHandle(const FItemHandle& ItemHandle) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	FItemHandle FindItemByData(const UItemInformation* ItemData) const;

	/**
	 * Attempt to add an item to the inventory
	 * @param Item - Item to add
	 * @return Whether the item could be added to the inventory
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	virtual bool AddToInventory(const FInventoryContents& Item, FItemHandle& OutItemHandle);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	virtual void EmptyInventory();

	//** Whether an item can be added to the inventory */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool CanAddToInventory(const FInventoryContents& Item) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void GetInventory(TArray<FInventoryContents>& OutInventory) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	void SetInventory(const TArray<FInventoryContents>& NewInventory);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	FItemHandle GenerateUniqueHandle() const;

	UFUNCTION(BlueprintCallable, Category="Inventory", meta=(AutoCreateRefTerm="DynamicData"))
	virtual FInventoryContents GenerateItem(UItemInformation* ItemInfo, const FInstancedStruct& DynamicData, int Count = 1) const;
	
protected:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Inventory, Category="Inventory")
	TArray<FInventoryContents> Inventory;
	
	UFUNCTION()
	virtual void OnRep_Inventory();
public:
	UPROPERTY(BlueprintAssignable)
	FInventoryChange OnInventoryChange;
};
