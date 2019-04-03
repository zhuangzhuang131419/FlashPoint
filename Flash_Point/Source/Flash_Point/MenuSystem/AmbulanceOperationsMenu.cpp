// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbulanceOperationsMenu.h"
#include "Components/Button.h"
#include "FireFighterPawn.h"
#include "Ambulance.h"
#include "Gameboard.h"

bool UAmbulanceOperationsMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (Success)
	{
		if (!ensure(OperationButton1)) { return false; }
		OperationButton1->OnClicked.AddDynamic(this, &UAmbulanceOperationsMenu::GetInAmbulance);

		if (!ensure(OperationButton2)) { return false; }
		OperationButton2->OnClicked.AddDynamic(this, &UAmbulanceOperationsMenu::GetOutAmbulance);

		if (!ensure(OperationButton3)) { return false; }
		OperationButton3->OnClicked.AddDynamic(this, &UAmbulanceOperationsMenu::CallAmbulance);
	}
	return true;
}

void UAmbulanceOperationsMenu::GetInAmbulance()
{
	UE_LOG(LogTemp, Warning, TEXT("Get in ambulance has been clicked."));
	if (ensure(fireFighterPawn))
	{
		if (ensure(fireFighterPawn->IsWithAmbulance()))
		{
			AGameBoard* playingBoard = fireFighterPawn->GetPlayingBoard();
			if (ensure(playingBoard))
			{
				AAmbulance* ambulanceOnBoard = playingBoard->GetAmbulance();
				if (ensure(ambulanceOnBoard))
				{
					ambulanceOnBoard->GetPassengers()->Add(fireFighterPawn);
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

void UAmbulanceOperationsMenu::GetOutAmbulance()
{
	UE_LOG(LogTemp, Warning, TEXT("Get out ambulance has been clicked.")); 
	/*
	if (ensure(fireFighterPawn))
	{
		if (!fireFighterPawn->IsInCar()) { return; }

		AGameBoard* playingBoard = fireFighterPawn->GetPlayingBoard();
		if (ensure(playingBoard))
		{
			// TODO
		}
	}
	*/
}

void UAmbulanceOperationsMenu::CallAmbulance()
{
	UE_LOG(LogTemp, Warning, TEXT("Call ambulance has been clicked."));
	this->SetVisibility(ESlateVisibility::Collapsed);
}
