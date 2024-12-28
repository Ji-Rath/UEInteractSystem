// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usable.h"
#include "Usable_GameplayAbility.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FUsable_GameplayAbility : public FUsableAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	TSubclassOf<UGameplayAbility> Ability;

	virtual void ExecuteAction(const FItemHandle& ItemHandle, AActor* Actor) const override;
	virtual void FinishExecuteAction(const FItemHandle& ItemHandle, AActor* Actor) const override;
};
