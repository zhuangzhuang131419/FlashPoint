// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SavePanel.generated.h"

class UButton;
class UEditableTextBox;
class AGameBoard;

/**
 * 
 */
UCLASS()
class FLASH_POINT_API USavePanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USavePanel(const FObjectInitializer& ObjectInitializer);
	bool Initialize() override;
	
protected:
	// BINDED WIDGETS
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UEditableTextBox* SavedGameName = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* YesButton = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* CancelButton = nullptr;
	
	// BINDED FUNCTIONS
	UFUNCTION()
	void OnYesButtonClicked();
	UFUNCTION()
	void OnCancelButtonClicked();

};
