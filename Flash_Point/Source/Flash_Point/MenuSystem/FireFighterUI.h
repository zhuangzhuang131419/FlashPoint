// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/MultiLineEditableText.h"
#include "GameBoard.h"
#include "FireFighterUI.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API UFireFighterUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	// BINDED WIDGETS
	// A text box to add text on during chat
	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableText* LobbyChatText;
	
	// FIELDS
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	AGameBoard* gameBoard = nullptr;

};
