// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemAction.generated.h"

struct FItemHandle;

/**
 * Executes an items action
 */
UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class INTERACTIONSYSTEM_API UItemAction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ItemAction", BlueprintPure = false)
	void ExecuteAction(const FItemHandle& ItemHandle, AActor* Actor) const;
	virtual void ExecuteAction_Implementation(const FItemHandle& ItemHandle, AActor* Actor) const {};
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ItemAction", BlueprintPure = false)
	void FinishExecuteAction(const FItemHandle& ItemHandle, AActor* Actor) const;
	virtual void FinishExecuteAction_Implementation(const FItemHandle& ItemHandle, AActor* Actor) const {};

	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif
};
