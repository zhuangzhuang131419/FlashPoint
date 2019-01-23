// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API AFPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFPPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
};
