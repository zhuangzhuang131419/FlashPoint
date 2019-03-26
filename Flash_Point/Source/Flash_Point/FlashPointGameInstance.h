// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FlashPointSaveGame.h"
#include "GeneralTypes.h"
#include "FlashPointGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API UFlashPointGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// UPROPERTY(Category = "GameSaving/Loading")	
	// void saveGame();
	// UPROPERTY(Category = "GameSaving/Loading")
	// void loadGame();


	// TSubclassOf<USaveGame> saveGameClass;
	
	// A function to produce a random indicator from the randomIndicators
	UFUNCTION(BlueprintCallable, Category = "Map Generation")
	FSpawnIndicator GetRandomMap();
	UFUNCTION(BlueprintCallable, Category = "Map Generation")
	FSpawnIndicator GetSpecificMap(int32 index);

protected:
	// FIELDS
	// randomly select a map indicator from preset
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Game Data Presets")
	TArray<FSpawnIndicator> randomIndicators;
};
