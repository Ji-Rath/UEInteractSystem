#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inventory/Pickupable.h"
#include "ItemUsable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemUsable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONSYSTEM_API IItemUsable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * @brief Interface function used to have items used on interactables
	 * @param User The actor who is using the item
	 * @param Item The item data being used
	 * @return Whether the item used was successful
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool OnItemUse(AActor* User, UItemData* Item);
};