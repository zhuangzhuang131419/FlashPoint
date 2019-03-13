// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/WidgetComponent.h"
#include "MenuSystem/FireFighterStatus.h"
#include "FireFighterUI.generated.h"

class AGameBoard;
class AFireFighterPawn;
class AFPPlayerController;

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
	void NotifyTurnBegin();
	UFUNCTION(BlueprintImplementableEvent, Category = "Take Turn")
	void NotifyTurnEnd();
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

protected:
	// BINDED WIDGETS
	// A text box to add text on during chat
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UMultiLineEditableTextBox* LobbyChatText = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UWidget* YourTurnPrompt = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UWidget* AbilityBar = nullptr;
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
	UImage* HazmatIcon = nullptr;
	// Player status showing

	// FIELDS
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	AGameBoard* gameBoard = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	AFireFighterPawn* localPawn = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	AFPPlayerController* localPlayer = nullptr;
};
