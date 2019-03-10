// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/WidgetComponent.h"
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
	// FUNCTIONS
	UFUNCTION(BlueprintImplementableEvent, Category = "Take Turn")
	void NotifyTurnBegin();
	// Getter and setter for related fields
	AGameBoard* GetGameBoard();
	void SetGameBoard(AGameBoard* inGameBoard);
	AFireFighterPawn* GetRelatedPawn();
	void SetRelatedPawn(AFireFighterPawn* inPawn);
	AFPPlayerController* GetRelatedPlayer();
	void SetRelatedPlayer(AFPPlayerController* inPlayer);

protected:
	// BINDED WIDGETS
	// A text box to add text on during chat
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UMultiLineEditableTextBox* LobbyChatText = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UWidget* YourTurnPrompt = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UWidget* AbilityBar = nullptr;

	// FIELDS
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	AGameBoard* gameBoard = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	AFireFighterPawn* localPawn = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	AFPPlayerController* localPlayer = nullptr;

};
