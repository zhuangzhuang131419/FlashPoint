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
	

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;

	// Widget class
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<UUserWidget> CreateNewGameMenu = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<UUserWidget> JoinGameMenu = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<UUserWidget> LoadGameMenu = nullptr;

	UPROPERTY(BluePrintReadWrite, Category = "Setup")
	TSubclassOf<UUserWidget> OptionMenu = nullptr;

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
	
};
