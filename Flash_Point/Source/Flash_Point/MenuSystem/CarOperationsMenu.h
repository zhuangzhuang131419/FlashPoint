// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CarOperationsMenu.generated.h"

class UTextBlock;
class UVerticalBox;
class AFPPlayerController;
class AFireFighterPawn;
/**
 * 
 */
UCLASS()
class FLASH_POINT_API UCarOperationsMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	// UCarOperationsMenu(const FObjectInitializer& ObjectInitializer);

	void AssociatePlayer(AFPPlayerController* inPlayer);
	
protected:
	bool Initialize();

	// BINDED WIDGETS
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UVerticalBox* MenuList;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UTextBlock* OperationText1 = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* OperationButton1 = nullptr;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UTextBlock* OperationText2 = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* OperationButton2 = nullptr;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UTextBlock* OperationText3 = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* OperationButton3 = nullptr;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UTextBlock* OperationText4 = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* OperationButton4 = nullptr;

	// FIELDS
	UPROPERTY(BlueprintReadWrite)
	AFPPlayerController* myOwner = nullptr;

	UPROPERTY(BlueprintReadWrite)
	AFireFighterPawn* fireFighterPawn = nullptr;

	UFUNCTION()
	void Cancel();
};
