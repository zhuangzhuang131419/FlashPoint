// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPPlayerController.generated.h"

UENUM(BlueprintType)
enum class EGameOperations : uint8
{
	PlaceFireFighter,
	Move,
	ChopWall,
	ExtinguishFire,
	Carry,
    OpenDoor,
	None
};

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
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetMove();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SetChopWall();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetExtinguishFire();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetCarry();
	AFPPlayerController();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	EGameOperations CurrentOperation = EGameOperations::None;
	
	
};
