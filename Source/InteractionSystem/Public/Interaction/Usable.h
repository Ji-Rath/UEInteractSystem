// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/ItemData.h"
#include "Usable.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIONSYSTEM_API UUsable : public UItemInformation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	TSubclassOf<UObject> Action;

	template <typename T>
	T* GetAction()
	{
		return Cast<T>(Action);
	}
};
