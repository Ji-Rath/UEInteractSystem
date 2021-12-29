#pragma once

#include "CoreMinimal.h"
#include "Interaction/Interactable.h"
#include "Pickupable.generated.h"

class UTriggerComponent;
class UStaticMeshComponent;

UCLASS()
class INTERACTIONSYSTEM_API APickupable : public AInteractable
{
	GENERATED_BODY()
	
public:
	APickupable();

	virtual void OnInteract_Implementation(AActor* Interactor) override;

	/** Add item to interactors inventory */
	UFUNCTION(BlueprintCallable)
	void PickupItem(AActor* Interactor);

	/**
	 * Called when using equipped item
	 * @return void
	*/
	UFUNCTION(BlueprintNativeEvent)
	void OnUseItem(AActor* User);

	UPROPERTY(EditAnywhere, Category = "Pickupable")
	int Amount = 1;

	/* Item data that contains name, description, etc */
	UPROPERTY(EditAnywhere, Category = "Interaction|Data")
	UItemData* ItemData = nullptr;

	/** Returns the item data */
	UFUNCTION(BlueprintCallable, Category = "Interaction|Data")
	UItemData* GetItemData() const;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;
};