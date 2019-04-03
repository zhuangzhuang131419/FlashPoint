// Fill out your copyright notice in the Description page of Project Settings.

#include "CarOperationsMenu.h"
#include "Components/Button.h"
#include "FireFighterPawn.h"
#include "FPPlayerController.h"

void UCarOperationsMenu::AssociatePlayer(AFPPlayerController * inPlayer)
{
	myOwner = inPlayer;
	if (ensure(myOwner))
	{
		fireFighterPawn = Cast<AFireFighterPawn>(myOwner->GetPawn());
	}
}

bool UCarOperationsMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (Success)
	{
		if (!ensure(OperationButton4)) { return false; }
		OperationButton4->OnClicked.AddDynamic(this, &UCarOperationsMenu::Cancel);
	}
	return true;
}

void UCarOperationsMenu::Cancel()
{
	UE_LOG(LogTemp, Warning, TEXT("Cancel has been clicked."));
	SetVisibility(ESlateVisibility::Collapsed);
}
