// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/CarOperationsMenu.h"
#include "FireEngineOperationsMenu.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API UFireEngineOperationsMenu : public UCarOperationsMenu
{
	GENERATED_BODY()
	
protected:
	bool Initialize();

	UFUNCTION()
	void FireDeckGun();
	
	
};
