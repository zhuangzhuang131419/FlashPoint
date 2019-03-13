// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GeneralTypes.h"
#include "Image.h"
#include "FireFighterStatus.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API UFireFighterStatus : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Teamate Status")
	void ChangeRolePic(ERoleType inRole);
	UFUNCTION(BlueprintCallable, Category = "Teamate Status")
	void AdjustAPBar(int32 cur, int32 max);
	UFUNCTION(BlueprintCallable, Category = "Teamate Status")
	void ShowPlayerName(FString name);
	
protected:
	// AP bar for the player's firefighter
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UProgressBar* TeammateAPBar = nullptr;
	// Name of the team mate
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UTextBlock* PlayerName = nullptr;
	// Role pic of the teammate
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UImage* TeamateRolePic = nullptr;
	
	
};
