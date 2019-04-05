// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GeneralTypes.h"
#include "OptionPrompt.generated.h"

class UTextBlock;
class AFireFighterPawn;
class AFPPlayerController;

/**
 * 
 */
UCLASS()
class FLASH_POINT_API UOptionPrompt : public UUserWidget
{
	GENERATED_BODY()

public:
	void AssociatePlayer(class AFPPlayerController* inPlayer);

	// Getter and setter
	UTextBlock* GetPromptText() { return PromptText; }

	// Prompt the operation to player
	void PromptOperation(EOptionPromptType operation, FString promptText);

	EOptionPromptType optionType;

protected:
	// BINDED WIDGETS
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UTextBlock* PromptText = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* AcceptButton = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* RefuseButton = nullptr;

	// FIELDS
	UPROPERTY(BlueprintReadWrite)
	AFPPlayerController* myOwner = nullptr;

	UPROPERTY(BlueprintReadWrite)
	AFireFighterPawn* fireFighterPawn = nullptr;

	bool Initialize();

	UFUNCTION()
	void Accept();
	void DodgeAccept();
	void CommandDoorAccept();
	void CommandTileAccept();
	void FireDeckGunAccept();

	UFUNCTION()
	void Refuse();
	void DodgeRefuse();
	void CommandDoorRefuse();
	void CommandTileRefuse();
	void FireDeckGunRefuse();
};
