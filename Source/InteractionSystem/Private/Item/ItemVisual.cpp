// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemVisual.h"

#include "Item/ItemData.h"

// Sets default values
AItemVisual::AItemVisual()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Visual"));
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void AItemVisual::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemVisual::SetItem(const UItemInformation* ItemInfo)
{
	MeshComp->SetStaticMesh(ItemInfo->ItemMesh.Get());
}
