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

		if (!ensure(OperationButton4)) { return false; }
		OperationButton4->OnClicked.AddDynamic(this, &UAmbulanceOperationsMenu::DriveAmbulance);
	}
	return Success;
}

void UAmbulanceOperationsMenu::GetInAmbulance()
{
	UE_LOG(LogTemp, Warning, TEXT("Get in ambulance has been clicked."));
	if (ensure(fireFighterPawn))
	{
		if (ensure(fireFighterPawn->IsWithAmbulance()))
		{
			AFPPlayerController* localPlayer = Cast<AFPPlayerController>(fireFighterPawn->GetController());
			if (ensure(localPlayer))
			{
				localPlayer->ServerGetInAmbulance(fireFighterPawn);
			}
			UE_LOG(LogTemp, Warning, TEXT("Get in car"));
			this->SetVisibility(ESlateVisibility::Collapsed);
			fireFighterPawn->DecolAdjust(true);
		}
	}
}

void UAmbulanceOperationsMenu::GetOutAmbulance()
{
	UE_LOG(LogTemp, Warning, TEXT("Get out ambulance has been clicked.")); 
	if (ensure(fireFighterPawn))
	{
		if (!fireFighterPawn->IsInCar()) { return; }
		AFPPlayerController* localPlayer = Cast<AFPPlayerController>(fireFighterPawn->GetController());
		if (ensure(localPlayer))
		{
			localPlayer->SetCurrentOperation(EGameOperations::GetOutAmbulance);
			this->SetVisibility(ESlateVisibility::Collapsed);
			fireFighterPawn->DecolAdjust(false);
		}
	}
}

void UAmbulanceOperationsMenu::CallAmbulance()
{
	UE_LOG(LogTemp, Warning, TEXT("Call ambulance has been clicked."));
	if (ensure(fireFighterPawn) && fireFighterPawn->GetFireFighterRole() != ERoleType::RescueDog)
	{
		// if (!fireFighterPawn->IsInCar()) { return; }
		AFPPlayerController* localPlayer = Cast<AFPPlayerController>(fireFighterPawn->GetController());
		if (ensure(localPlayer))
		{
			localPlayer->SetCurrentOperation(EGameOperations::Radio);
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UAmbulanceOperationsMenu::DriveAmbulance()
{
	UE_LOG(LogTemp, Warning, TEXT("Drive ambulance has been clicked."));
	if (ensure(fireFighterPawn) && fireFighterPawn->GetFireFighterRole() != ERoleType::RescueDog)
	{
		if (!fireFighterPawn->IsInCar()) { return; }
		AFPPlayerController* localPlayer = Cast<AFPPlayerController>(fireFighterPawn->GetController());
		if (ensure(localPlayer))
		{
			localPlayer->SetCurrentOperation(EGameOperations::DriveAmbulance);
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
