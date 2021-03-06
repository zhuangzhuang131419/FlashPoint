// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GeneralTypes.h"
#include "MainMenu.generated.h"

class UMapOverviewPanel;
class UVerticalBox;
class ULobbyBar;
class USavedGameBar;
class UTextBlock;
class UFlashPointGameInstance;

/**
 * 
 */
UCLASS()
class FLASH_POINT_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);
	void ClearAllLobbyList();
	void ClearAllSavedList();
	void EnableRefreshButton(bool enabled);
	void EnableCreateLobby(bool enabled);
	void InsertLobbyBar(FGameLobbyInfo inInfo, int32 inIndex, int32 joinedPlayers);
	void ShowRefreshing(bool isRefreshing);
	void ShowLoading(bool isLoading);
	void ShowJoinStatus(bool hasFailed);
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetGameInst(UFlashPointGameInstance* inGameInst) { gameInst = inGameInst; }

protected:
	virtual bool Initialize() override;

	// FIELDS
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	UFlashPointGameInstance* gameInst = nullptr;

	// Widget class
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ULobbyBar> LobbyBarClass = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<USavedGameBar> SavedBarClass = nullptr;

	// BINDED WIDGETS
	UPROPERTY(meta = (BindWidget))
	UButton* New_Game = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* Join_Game = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* Load_Game = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* Option_Button = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* Exit_Game = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* RefreshLobbies = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* CreateNewGame = nullptr;
	UPROPERTY(meta = (BindWidget))
	UMapOverviewPanel* JoinMapOverview = nullptr;
	UPROPERTY(meta = (BindWidget))
	UMapOverviewPanel* LoadMapOverview = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* LobbyArrangement = nullptr;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* LoadArrangement = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RefreshingText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoadingText = nullptr;

	// Button functions
	// New Game button
	UFUNCTION()
	void OnNewGameClicked();
	UFUNCTION()
	void OnJoinGameClicked();
	UFUNCTION()
	void OnLoadGameClicked();
	UFUNCTION()
	void OnOptionClicked();
	UFUNCTION()
	void OnExitClicked();
	UFUNCTION()
	void OnRefreshLobbies();
	
};
