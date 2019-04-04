// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GeneralTypes.h"
#include "MapOverviewPanel.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API UMapOverviewPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMapOverviewPanel(const FObjectInitializer& ObjectInitializer);

	// FUNCTIONS
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Map Overview")
	void ShowMapInfo(EGameMap inMap, const FString& detailedInfo);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Map Overview")
	void ClearMapInfo();
	
protected:
	// FIELDS

	
};
