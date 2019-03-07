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

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChopWall(AWall* wall);
	// A function to update wall mesh on client
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void ClientWallMeshUpdate(AWall* wall, bool chopedDown);


	UFUNCTION(BlueprintCallable)
	AVictim* GetCarriedVictim();

	UFUNCTION(BlueprintCallable)
	void SetCarriedVictim(AVictim* victim);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	EGameOperations CurrentOperation = EGameOperations::None;
	
private:
	AVictim* carriedVictim;
	
};
