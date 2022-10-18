#pragma once

#include "CoreMinimal.h"
#include "PlayerInteractComponent.h"
#include "AreaInteractComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UAreaInteractComponent : public UPlayerInteractComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> OverlappingActors;

protected:
	void HoverInteraction_Implementation(float DeltaTime) override;

};