// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/WidgetComponent.h"
#include "MenuSystem/FireFighterStatus.h"
#include "MenuSystem/ChatWidget.h"
#include "FireFighterUI.generated.h"

class AGameBoard;
class AFireFighterPawn;
class AFPPlayerController;
class UButton;
class USavePanel;

/**
 * 
 */
UCLASS()
class FLASH_POINT_API UFireFighterUI : public UUserWidget
{
	GENERATED_BODY()

public:
	// Constructor for the widget
	UFireFighterUI(const FObjectInitializer& ObjectInitializer);

	// FUNCTIONS
	UFUNCTION(BlueprintImplementableEvent, Category = "Take Turn")
	void EnableOperationWidgets();
	UFUNCTION(BlueprintImplementableEvent, Category = "Take Turn")
	void NotifyPlaceFirefighter();
	UFUNCTION(BlueprintImplementableEvent, Category = "Take Turn")
	void NotifyTurnBegin();
	UFUNCTION(BlueprintImplementableEvent, Category = "Take Turn")
	void NotifyTurnEnd();
	UFUNCTION(BlueprintImplementableEvent, Category = "Take Turn")
	void NotifyGameEnd(bool isWin);
	void PromptCommandStatus(EAcceptanceStatus acceptState);
	// Getter and setter for related fields
	AGameBoard* GetGameBoard();
	void SetGameBoard(AGameBoard* inGameBoard);
	AFireFighterPawn* GetRelatedPawn();
	void SetRelatedPawn(AFireFighterPawn* inPawn);
	AFPPlayerController* GetRelatedPlayer();
	void SetRelatedPlayer(AFPPlayerController* inPlayer);
	UFireFighterStatus* GetPlayerStatusUIOf(int32 playerID);
	// Set enable or disable carrying victim or hazmat victim
	UFUNCTION(BlueprintCallable, Category = "UI Synchronization")
	void ShowCarrying(bool isCarrying);
	UFUNCTION(BlueprintCallable, Category = "UI Synchronization")
	void ShowCarryHazmat(bool isCarrying);
	UFUNCTION(BlueprintCallable, Category = "UI Synchronization")
	void ShowLeading(bool isCarrying);
	UFUNCTION(BlueprintCallable, Category = "UI Synchronization")
	void EnableOperationPanels(bool enable);
	void BindChatManagerWithUI(AChatManager* inMan);
	void RelateChatUIWithPlayer(AFPPlayerController* inPlayer);
	void SetBeginTurnNotify(bool isBegin);
	void NotifySomeOneLeft();
	void NotifyPlaceCar(FString promptInfo);

protected:
	// BIDNING FUNCTIONS
	bool Initialize() override;
	UFUNCTION()
	void NotifyCrewChange();
	UFUNCTION()
	void OnExitClicked();
	UFUNCTION()
	void OnSaveClicked();

	// BINDED WIDGETS
	// A text box to add text on during chat
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UChatWidget* ChatBox = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UWidget* YourTurnPrompt = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UTextBlock* PromptText = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UWidget* AbilityBar = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UWidget* InventoryAndSkip = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* ConfirmTurnButton = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* SaveButton = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* ExitButton = nullptr;
	// teammate status
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UFireFighterStatus* P0Status = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UFireFighterStatus* P1Status = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UFireFighterStatus* P2Status = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UFireFighterStatus* P3Status = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UFireFighterStatus* P4Status = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UFireFighterStatus* P5Status = nullptr;
	// carrying victim or hazmat
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UImage* VictimIcon = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UImage* LeadingIcon = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UImage* HazmatIcon = nullptr;
	// Player status showing

	// FIELDS
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	AGameBoard* gameBoard = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	AFireFighterPawn* localPawn = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	AFPPlayerController* localPlayer = nullptr;
	bool isBeginOfTurn = false;
	bool isNotifyPlayerLeave = false;
	TSubclassOf<USavePanel> SavePanelClass = nullptr;
	USavePanel* saveGamePanel = nullptr;
};
