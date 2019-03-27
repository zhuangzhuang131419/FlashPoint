// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameBoard.h"
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
	UFlashPointSaveGame();

public:
	// Basic
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Basic")
	FString PlayerName = "";

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Basic")
	FString SaveSlotName;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Basic")
	int32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = GameBoard)
	uint32 health = 0;

	UPROPERTY(VisibleAnywhere, Category = GameBoard)
	TArray<ATile*> tiles;

	UPROPERTY(VisibleAnywhere, Category = GameBoard)
	AGameBoard* gameboard = nullptr;

	UPROPERTY()
	FDateTime PlayerSaveSlotDate;

};
