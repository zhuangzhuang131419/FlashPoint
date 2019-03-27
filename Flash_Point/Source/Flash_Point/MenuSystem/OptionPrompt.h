// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionPrompt.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API UOptionPrompt : public UUserWidget
{
	GENERATED_BODY()

public:
	void AssociatePlayer(class AFPPlayerController* inPlayer);

protected:
	// BINDED WIDGETS
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock* PromptText = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* AcceptButton = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* RefuseButton = nullptr;

	// FIELDS
	UPROPERTY(BlueprintReadWrite)
	AFPPlayerController* myOwner = nullptr;
	
	
};
