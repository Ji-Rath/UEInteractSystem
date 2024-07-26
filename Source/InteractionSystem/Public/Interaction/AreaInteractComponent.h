#pragma once

#include "CoreMinimal.h"
#include "InteractorComponent.h"
#include "AreaInteractComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UAreaInteractComponent : public UInteractorComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> OverlappingActors;

protected:
	virtual void UpdateHoverActor() override;

};