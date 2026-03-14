// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemVisual.generated.h"

class UItemInformation;

UCLASS()
class INTERACTIONSYSTEM_API AItemVisual : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemVisual();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="ItemVisual")
	TObjectPtr<UStaticMeshComponent> MeshComp;
	
public:
	UFUNCTION(BlueprintCallable, Category="ItemVisual")
	void SetItem(const UItemInformation* ItemInfo);
};
