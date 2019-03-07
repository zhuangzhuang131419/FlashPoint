// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdgeUnit.h"
#include "Components/StaticMeshComponent.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API ADoor : public AEdgeUnit
{
	GENERATED_BODY()
	
public:
	ADoor();

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	UStaticMeshComponent* DoorFrame;
	

protected:
	virtual void BeginPlay() override;

private:
	bool isOpened;

	

};
