// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemData.h"

FPrimaryAssetId UItemInformation::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FName("ItemInformation"), FPackageName::GetShortFName(GetOutermost()->GetName()));
}
