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
	// getter and setter for game type
	UFUNCTION(BlueprintCallable, Category = "Map Generation")
	EGameType GetGameType();
	UFUNCTION(Exec, BlueprintCallable, Category = "Map Generation")
	void SetGameType(EGameType inGameType);
	// Setting the game to be loaded
	UFUNCTION(BlueprintCallable, Category = "Map Generation")
	int32 GetLoadGameIndex();
	UFUNCTION(Exec, BlueprintCallable, Category = "Map Generation")
	void SetLoadGameIndex(int32 index);
	// load a certain map
	UFUNCTION(Exec, BlueprintCallable, Category = "Map Generation")
	void LoadMapOfIndex(int32 index);
	UFUNCTION(Exec, BlueprintCallable, Category = "Map Generation")
	FMapSaveInfo GetLoadedGame();

protected:
	// FIELDS
	// randomly select a map indicator from preset
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Game Data Presets")
	TArray<FSpawnIndicator> randomIndicators;
	// a field to store the game mode the player started
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Map Generation")
	EGameType gameType = EGameType::Family;
	// a field to store the game mode the player started
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Map Generation")
	int32 loadGameIndex = -1;
	// store the loaded map
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Save Load Related")
	FMapSaveInfo loadedMap;
};
