// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GeneralTypes.h"
#include "LobbyUI.generated.h"

class ACrewManager;
class AChatManager;
class ALobbyManager;
class AFPPlayerController;
class AFireFighterPawn;
class UChatWidget;
class USwitchRoleUI;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class FLASH_POINT_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	ULobbyUI(const FObjectInitializer& ObjectInitializer);

	bool Initialize() override;

	// FUNCTIONS
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Lobby Creation")
	void ShowAllLobbyInfo(FGameLobbyInfo inInfo);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Lobby Creation")
	void ShowSelectedRole(ERoleType inRole);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Lobby Creation")
	void UpdateDisplayedInfo(FPlayerLobbyInfo P0, FPlayerLobbyInfo P1, FPlayerLobbyInfo P2, FPlayerLobbyInfo P3, FPlayerLobbyInfo P4, FPlayerLobbyInfo P5);
	void BindChatManagerWithUI(AChatManager* inMan);
	void BindCrewManagerWithUI(ACrewManager* inMan);
	void BindLobbyManagerWithUI(ALobbyManager* inMan);
	void RelateChatUIWithPlayer(AFPPlayerController* inPlayer);
	void ChangeJoinStartButtonStatus(FString inMessage, bool isEnabled);
	void EnableRoleSelection(bool isEnabled);
	
protected:
	// Managers related to the lobby UI
	UPROPERTY(BlueprintReadWrite, Category = "Lobby Creation")
	ACrewManager* crewMan;
	UPROPERTY(BlueprintReadWrite, Category = "Lobby Creation")
	ALobbyManager* lobbyMan;
	
	// BINDED WIDGETS
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UChatWidget* ChatPanel = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	USwitchRoleUI* SelectRolePanel = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* ReadyOrStartButton = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* BackToMainMenu = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UTextBlock* JOSButtonText = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* SwitchRoleButton = nullptr;

	// binding functions to button
	UFUNCTION()
	void OnReadyOrStart();
	UFUNCTION()
	void OnBackToMainMenu();
	
};
