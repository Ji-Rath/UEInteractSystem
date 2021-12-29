#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "ToggleInteractable.generated.h"

UCLASS()
class INTERACTIONSYSTEM_API AToggleInteractable : public AInteractable
{
	GENERATED_BODY()
	
public:
	AToggleInteractable();
	
	/**
	 * Toggle the status of the interactable
	 * @warning Will not toggle more than once if bOneTimeInteraction is true
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ToggleInteract(AActor* Interactor);

	virtual bool Interact(AActor* Interactor) override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetState(AActor* Interactor, bool bOn);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool GetState() const;
	
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void ConstructInteractable();
	
	/** The current state of the interactable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bIsOn = false;
};

