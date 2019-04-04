// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GeneralTypes.h"
#include "LobbyBar.generated.h"

class UButton;
class UTextBlock;
class UBorder;
class UMapOverviewPanel;

/**
 * 
 */
UCLASS()
class FLASH_POINT_API ULobbyBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	ULobbyBar(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	// A function that changes the images with regard to if is saved game
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby Bar")
	void DisplayIsSaved(bool isSaved);
	// a function to initialize all the attributes of the lobby bar
	void InitializeAttributes(UMapOverviewPanel* inOverView, FGameLobbyInfo inInfo, int32 inIndex, int32 joinedPlayers);

protected:
	// BINDED WIDGETS
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LobbyNameText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* JoinedPlayerText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ModeText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InitialExplosionNum = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InitialHazmatNum = nullptr;
	UPROPERTY(meta = (BindWidget))
	UBorder* DummyBorder = nullptr;

	// WIDGET BINDING FUNCTIONS
	UFUNCTION()
	void OnJoinButton();
	UFUNCTION()
	void OnDownDummyBorder();
	UFUNCTION()
	void OnDoubleClickDummyBorder();

	// Associated attributes
	UMapOverviewPanel* mapOverview = nullptr;
	FGameLobbyInfo lobbyInfo;
	int32 joinIndex = -1;
	int32 capacity = -1;
	
};
