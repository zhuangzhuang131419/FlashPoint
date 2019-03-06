// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

	// Widget class
	UPROPERTY(BluePrintReadWrite, Category = "Setup")
	TSubclassOf<UUserWidget> CreateNewGameMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* New_Game = nullptr;

	// Button functions
	// New Game button
	void OnNewGameClicked();
	
};
