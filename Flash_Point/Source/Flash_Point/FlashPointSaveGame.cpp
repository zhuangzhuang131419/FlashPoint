// Fill out your copyright notice in the Description page of Project Settings.

#include "FlashPointSaveGame.h"

UFlashPointSaveGame::UFlashPointSaveGame()
{
	SaveSlotName = TEXT("TestSaveSlot");
	UserIndex = 0;
}

void UFlashPointSaveGame::SaveHealth()
{
	UFlashPointSaveGame* SaveGameInstance = Cast<UFlashPointSaveGame>(UGameplayStatics::CreateSaveGameObject(UFlashPointSaveGame::StaticClass()));
	SaveGameInstance->health = 20;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
}

void UFlashPointSaveGame::LoadHealth()
{
	UFlashPointSaveGame* LoadGameInstance = Cast<UFlashPointSaveGame>(UGameplayStatics::CreateSaveGameObject(UFlashPointSaveGame::StaticClass()));
	LoadGameInstance = Cast<UFlashPointSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	FString PlayerNameToDisplay = LoadGameInstance->PlayerName;
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, PlayerNameToDisplay);
	}*/
}




