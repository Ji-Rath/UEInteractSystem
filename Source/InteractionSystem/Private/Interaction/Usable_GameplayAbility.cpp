// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Usable_GameplayAbility.h"

#include "AbilitySystemComponent.h"

void FUsable_GameplayAbility::ExecuteAction(const FItemHandle& ItemHandle, AActor* Actor) const
{
	if (auto ASC = Actor->GetComponentByClass<UAbilitySystemComponent>())
	{
		ASC->PressInputID(1);
	}
}

void FUsable_GameplayAbility::FinishExecuteAction(const FItemHandle& ItemHandle, AActor* Actor) const
{
	if (auto ASC = Actor->GetComponentByClass<UAbilitySystemComponent>())
	{
		ASC->ReleaseInputID(1);
	}
}
