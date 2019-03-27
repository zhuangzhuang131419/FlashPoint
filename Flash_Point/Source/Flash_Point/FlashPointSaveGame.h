// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GeneralTypes.h"
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
	// Basic info for saving
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Basic")
	FString PlayerName = "";
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Basic")
	FString SaveSlotName;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Basic")
	int32 UserIndex;

	// An tarray of saved game structure
	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere, Category = "Save info")
	TArray<FMapSaveInfo> savedGames;
};
