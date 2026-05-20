// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemAction.h"
#include "Abilities/GameplayAbility.h"
#include "ItemAction_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIONSYSTEM_API UItemAction_GameplayAbility : public UItemAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemAction")
	TSubclassOf<UGameplayAbility> GameplayAbility;
	
	UPROPERTY()
	FGameplayAbilitySpecHandle SpecHandle;

protected:
	virtual void ExecuteAction_Implementation(const FItemHandle& ItemHandle, AActor* Actor) override;
	virtual void FinishExecuteAction_Implementation(const FItemHandle& ItemHandle, AActor* Actor) override;
};
