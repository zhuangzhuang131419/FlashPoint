// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "ViewPortCamera.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API AViewPortCamera : public ACameraActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void RelocateAndSetToViewPort(FVector location);

	
	
};
