// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/CarOperationsMenu.h"
#include "AmbulanceOperationsMenu.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API UAmbulanceOperationsMenu : public UCarOperationsMenu
{
	GENERATED_BODY()
	
protected:
	bool Initialize();

	UFUNCTION()
	void GetInAmbulance();
	UFUNCTION()
	void GetOutAmbulance();
	UFUNCTION()
	void CallAmbulance();
	UFUNCTION()
	void DriveAmbulance();
};
