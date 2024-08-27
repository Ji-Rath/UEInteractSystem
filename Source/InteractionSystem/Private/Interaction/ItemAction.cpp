// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/ItemAction.h"

UWorld* UItemAction::GetWorld() const
{
	if (GetOuter() == nullptr)
	{
		return nullptr;
	}

	// Special case for behavior tree nodes in the editor
	if (Cast<UPackage>(GetOuter()) != nullptr)
	{
		// GetOuter should return a UPackage and its Outer is a UWorld
		return Cast<UWorld>(GetOuter()->GetOuter());
	}

	// In all other cases...
	return GetOuter()->GetWorld();
}

bool UItemAction::ImplementsGetWorld() const
{
	return true;
}
