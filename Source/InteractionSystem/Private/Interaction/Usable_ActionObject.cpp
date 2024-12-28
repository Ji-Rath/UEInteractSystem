// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Usable_ActionObject.h"

#include "Interaction/ItemAction.h"

void FUsable_ActionObject::ExecuteAction(const FItemHandle& ItemHandle, AActor* Actor) const
{
	if (UItemAction* ActionObject = NewObject<UItemAction>(Actor, Action))
	{
		ActionObject->Execute(ItemHandle);
	}
}
