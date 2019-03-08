// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"
#include "GeneralTypes.h"
#include "FPPlayerController.generated.h"

class AVictim;
class AWall;
class ATile;
class ADoor;
class AEdgeUnit;
class AGameBoard;

/**
 * 
 */
UCLASS()
class FLASH_POINT_API AFPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Below functions are for changing operations
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	EGameOperations GetCurrentOperation();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetOpenDoor();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetNone();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetPlaceFireFighter();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SetRespawnFireFighter();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetMove();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SetChopWall();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetExtinguishFire();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetCarry();
	AFPPlayerController();

	// choped wall on server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChopWall(AWall* wall);
	// open door on server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOpenDoor(ADoor* door);

	UFUNCTION(BlueprintCallable)
	void DropVictim();

	UFUNCTION(BlueprintCallable)
	void CarryVictim();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	EGameOperations CurrentOperation = EGameOperations::None;
};
