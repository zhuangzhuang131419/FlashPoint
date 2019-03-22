// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "FlashPointSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API UFlashPointSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = GameBoard)
	uint32 health = 0;
	
	UFlashPointSaveGame();

	void SaveHealth();
	void LoadHealth();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
	
};
