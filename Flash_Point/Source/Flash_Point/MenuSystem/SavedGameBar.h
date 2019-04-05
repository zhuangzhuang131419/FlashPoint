// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GeneralTypes.h"
#include "SavedGameBar.generated.h"

class UButton;
class UTextBlock;
class UBorder;
class UMapOverviewPanel;

/**
 * 
 */
UCLASS()
class FLASH_POINT_API USavedGameBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USavedGameBar(const FObjectInitializer& ObjectInitializer);
	virtual bool Initialize() override;
	// a function to initialize all the attributes of the loaded game bar
	void InitializeAttributes(UMapOverviewPanel* inOverView, FMapSaveInfo inInfo);
	
protected:
	// BINDED WIDGETS
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SavedGameText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DateText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ModeText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LostVictimsText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BoardHealthText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* LoadGameButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UBorder* DummyBorder = nullptr;

	// WIDGET BINDING FUNCTIONS
	UFUNCTION()
	void OnLoadButton();
	UFUNCTION()
	void OnDownDummyBorder();
	UFUNCTION()
	void OnDoubleClickDummyBorder();

	// FIELDS
	UMapOverviewPanel* mapOverview = nullptr;
	FMapSaveInfo savedInfo;

};
