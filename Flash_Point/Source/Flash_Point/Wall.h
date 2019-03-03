// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdgeUnit.h"
#include "Engine/World.h"
#include "Wall.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API AWall : public AEdgeUnit
{
	GENERATED_BODY()

public:
	AWall();

	UPROPERTY(EditAnyWhere, Category = "SetUp")
	bool isChoped;

protected:
	// Cursor clicked method
	UFUNCTION()
	void OnCursorClicked(UPrimitiveComponent* Component);
	
	
	
};
