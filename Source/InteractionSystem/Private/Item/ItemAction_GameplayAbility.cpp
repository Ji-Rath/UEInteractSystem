// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemAction_GameplayAbility.h"

#include "AbilitySystemComponent.h"

void UItemAction_GameplayAbility::ExecuteAction_Implementation(const FItemHandle& ItemHandle, AActor* Actor)
{
	UAbilitySystemComponent* ASC = Actor->FindComponentByClass<UAbilitySystemComponent>();
	if (ensure(ASC))
	{
		FGameplayAbilitySpec Spec(GameplayAbility);
		Spec.InputPressed = true;
		SpecHandle = ASC->GiveAbility(Spec);
		
		ASC->TryActivateAbility(SpecHandle);
		ASC->SetRemoveAbilityOnEnd(SpecHandle);
	}
}

void UItemAction_GameplayAbility::FinishExecuteAction_Implementation(const FItemHandle& ItemHandle, AActor* Actor)
{
	UAbilitySystemComponent* ASC = Actor->FindComponentByClass<UAbilitySystemComponent>();
	FGameplayAbilitySpec* Spec = ASC ? ASC->FindAbilitySpecFromHandle(SpecHandle) : nullptr;
	
	if (Spec)
	{
		ASC->AbilitySpecInputReleased(*Spec);
		
		// Similar jank to locally pressing input within ASC
		const FGameplayAbilityActivationInfo& ActivationInfo = Spec->GetAbilityInstances().IsEmpty() ? Spec->ActivationInfo : Spec->GetAbilityInstances().Last()->GetCurrentActivationInfoRef();
		ASC->InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec->Handle, ActivationInfo.GetActivationPredictionKey());
	}
}
