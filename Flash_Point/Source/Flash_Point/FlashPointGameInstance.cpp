// Fill out your copyright notice in the Description page of Project Settings.

#include "FlashPointGameInstance.h"
#include "Kismet/GameplayStatics.h"

//void UFlashPointGameInstance::saveGame()
//{
//	UFlashPointSaveGame* flashPointSaveGame = Cast<UFlashPointSaveGame>(UGameplayStatics::LoadGameFromSlot(FString("SaveSlot"), 0));
//	if (flashPointSaveGame == nullptr)
//	{
//		flashPointSaveGame = Cast<UFlashPointSaveGame>(UGameplayStatics::CreateSaveGameObject(saveGameClass));
//	}
//	UGameplayStatics::SaveGameToSlot(flashPointSaveGame, FString("SaveSlot"), 0);
//}
//
//void UFlashPointGameInstance::loadGame()
//{
//	UFlashPointSaveGame* flashPointSaveGame = Cast<UFlashPointSaveGame>(UGameplayStatics::LoadGameFromSlot(FString("SaveSlot"), 0));
//	if (flashPointSaveGame)
//	{
//
//	}
//}

FSpawnIndicator UFlashPointGameInstance::GetRandomMap() {
	// randomize a integer to return as random map's index
	if (randomIndicators.Num() > 0) {
		int32 randNum = FMath::RandRange(0, randomIndicators.Num() - 1);
		return randomIndicators[randNum];
	}
	else {
		return FSpawnIndicator();
	}
}

FSpawnIndicator UFlashPointGameInstance::GetSpecificMap(int32 index)
{
	if (index < 0 || index >= randomIndicators.Num()) {
		return FSpawnIndicator();
	}
	else {
		return randomIndicators[index];
	}
}
