// Fill out your copyright notice in the Description page of Project Settings.

#include "FireEngineOperationsMenu.h"
#include "Components/Button.h"
#include "FireFighterPawn.h"
#include "FireEngine.h"
#include "Gameboard.h"

bool UFireEngineOperationsMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (Success)
	{
		if (!ensure(OperationButton1)) { return false; }
		OperationButton1->OnClicked.AddDynamic(this, &UFireEngineOperationsMenu::GetInFireEngine);

		if (!ensure(OperationButton3)) { return false; }
		OperationButton3->OnClicked.AddDynamic(this, &UFireEngineOperationsMenu::FireDeckGun);
	}
	return true;
}

void UFireEngineOperationsMenu::GetInFireEngine()
{
	UE_LOG(LogTemp, Warning, TEXT("Get in fire engine has been clicked."));
	if (ensure(fireFighterPawn))
	{
		if (ensure(fireFighterPawn->IsWithEngine()))
		{
			AGameBoard* playingBoard = fireFighterPawn->GetPlayingBoard();
			if (ensure(playingBoard))
			{
				AFireEngine* fireEngineOnBoard = playingBoard->GetFireEngine();
				if (ensure(fireEngineOnBoard))
				{
					fireEngineOnBoard->GetPassengers()->Add(fireFighterPawn);
					fireFighterPawn->SetVisibility(false);
					UE_LOG(LogTemp, Warning, TEXT("Get in car"));
				}
			}
		}
	}
}

void UFireEngineOperationsMenu::FireDeckGun()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire deck gun has been clicked."));
	
	if (ensure(fireFighterPawn))
	{
		if (ensure(fireFighterPawn->IsWithEngine()))
		{
			AFPPlayerController* localPlayer = Cast<AFPPlayerController>(fireFighterPawn->GetController());
			// localPlayer->ServerFireDeckGun(fireFighterPawn);
			this->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
