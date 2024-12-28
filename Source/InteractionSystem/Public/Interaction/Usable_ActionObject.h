// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usable.h"
#include "Usable_ActionObject.generated.h"

class UItemAction;

USTRUCT(BlueprintType)
struct FUsable_ActionObject : public FUsableAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	TSubclassOf<UItemAction> Action;

	virtual void ExecuteAction(const FItemHandle& ItemHandle, AActor* Actor) const override;
};
