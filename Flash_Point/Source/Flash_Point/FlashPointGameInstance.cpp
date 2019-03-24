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
