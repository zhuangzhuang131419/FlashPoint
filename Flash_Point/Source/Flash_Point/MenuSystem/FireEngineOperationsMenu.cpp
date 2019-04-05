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

		if (!ensure(OperationButton2)) { return false; }
		OperationButton2->OnClicked.AddDynamic(this, &UFireEngineOperationsMenu::GetOutFireEngine);

		if (!ensure(OperationButton3)) { return false; }
		OperationButton3->OnClicked.AddDynamic(this, &UFireEngineOperationsMenu::FireDeckGun);

		if (!ensure(OperationButton4)) { return false; }
		OperationButton4->OnClicked.AddDynamic(this, &UFireEngineOperationsMenu::DriveFireEngine);
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
					AFPPlayerController* localPlayer = Cast<AFPPlayerController>(fireFighterPawn->GetController());
					if (ensure(localPlayer))
					{
						localPlayer->ServerGetInCar(fireFighterPawn);
					}
					UE_LOG(LogTemp, Warning, TEXT("Get in car"));
					this->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}
}

void UFireEngineOperationsMenu::GetOutFireEngine()
{
	UE_LOG(LogTemp, Warning, TEXT("Get out fire engine has been clicked."));
	if (ensure(fireFighterPawn))
	{
		if (!fireFighterPawn->IsInCar()) { return; }
		AFPPlayerController* localPlayer = Cast<AFPPlayerController>(fireFighterPawn->GetController());
		if (ensure(localPlayer))
		{
			localPlayer->SetCurrentOperation(EGameOperations::GetOutFireEngine);
			this->SetVisibility(ESlateVisibility::Collapsed);
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
			localPlayer->ServerFireDeckGun(fireFighterPawn);
			this->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UFireEngineOperationsMenu::DriveFireEngine()
{
	UE_LOG(LogTemp, Warning, TEXT("Drive fire engine has been clicked."));
	if (ensure(fireFighterPawn))
	{
		if (!fireFighterPawn->IsInCar()) { return; }
		AFPPlayerController* localPlayer = Cast<AFPPlayerController>(fireFighterPawn->GetController());
		if (ensure(localPlayer))
		{
			localPlayer->SetCurrentOperation(EGameOperations::DriveFireEngine);
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
