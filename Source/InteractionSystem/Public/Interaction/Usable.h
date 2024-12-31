// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/ItemData.h"
#include "StructUtils/InstancedStruct.h"
#include "Usable.generated.h"

struct FItemHandle;

USTRUCT(NotBlueprintType)
struct FUsableAction
{
	GENERATED_BODY()
public:
	virtual void ExecuteAction(const FItemHandle& ItemHandle, AActor* Actor) const {};
	virtual void FinishExecuteAction(const FItemHandle& ItemHandle, AActor* Actor) const {};
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class INTERACTIONSYSTEM_API UUsable : public UItemInformation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	TInstancedStruct<FUsableAction> ActionConfig;
};
