// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemAction.generated.h"

struct FItemHandle;

/**
 * Executes an items action
 */
UCLASS(Abstract, Blueprintable)
class INTERACTIONSYSTEM_API UItemAction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Execute(const FItemHandle& Item) const;

	virtual UWorld* GetWorld() const override;

	virtual bool ImplementsGetWorld() const override;
};
