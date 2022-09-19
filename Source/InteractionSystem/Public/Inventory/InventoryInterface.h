// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

class UItemData;
// This class does not need to be modified.
UINTERFACE()
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONSYSTEM_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Remove an item from current inventory */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RemoveFromInventory(const UItemData* Item, const int Count = 1);

	/**
	 * Attempt to add an item to the inventory
	 * @param Item - Item to add
	 * @param Count - Amount of item
	 * @return Whether the item could be added to the inventory
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool AddToInventory(UItemData* Item, const int Count);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool DoesItemExist(UItemData* Item, int& OutIndex);
};
