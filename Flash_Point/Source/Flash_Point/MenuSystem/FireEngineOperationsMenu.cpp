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
	return Success;
}

void UFireEngineOperationsMenu::GetInFireEngine()
{
	UE_LOG(LogTemp, Warning, TEXT("Get in fire engine has been clicked."));
	if (ensure(fireFighterPawn))
	{
		if (ensure(fireFighterPawn->IsWithEngine()))
		{
			AFPPlayerController* localPlayer = Cast<AFPPlayerController>(fireFighterPawn->GetController());
			if (ensure(localPlayer))
			{
				localPlayer->ServerGetInFireEngine(fireFighterPawn);
			}
			UE_LOG(LogTemp, Warning, TEXT("Get in car"));
			this->SetVisibility(ESlateVisibility::Collapsed);
			fireFighterPawn->DecolAdjust(true);
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
			fireFighterPawn->DecolAdjust(false);
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
			if (!ensure(localPlayer)) { return; }
			AGameBoard* board = localPlayer->GetGameBoard();
			if (ensure(board) && ensure(board->GetFireEngineLocA()))
			{
				int32 startx = -1;
				int32 starty = -1;
				// check no fire fighter is in the this quadrant
				board->GetFireEngineLocA()->GetLocation(startx, starty);

				if (startx <= 4) { startx = 1; }
				else { startx = 5; }
				if (starty <= 3) { starty = 1; }
				else { starty = 4; }
				UE_LOG(LogTemp, Warning, TEXT("x: %d, y: %d"), startx, starty);

				for (int32 x = startx; x <= startx + 3; x++)
				{
					for (int32 y = starty; y <= starty + 2; y++)
					{
						// UE_LOG(LogTemp, Warning, TEXT("Checking tile: %d, %d, %s"), x, y, *board->GetboardTiles()[x * board->GetBoardLength() + y]->GetName());
						if (board->GetboardTiles()[x * board->GetBoardLength() + y]->GetPlacedFireFighters()->Num() > 0)
						{
							this->SetVisibility(ESlateVisibility::Collapsed);
							UE_LOG(LogTemp, Warning, TEXT("Unable to use fire engine. Some one is in the quadrant"));
							return;
						}
					}
				}

				ATile* targetTile = board->GenerateRandomPositionInQuadrant();
				if (ensure(targetTile))
				{
					UE_LOG(LogTemp, Warning, TEXT("target tile %s"), *targetTile->GetName());
					if (fireFighterPawn->GetFireFighterRole() == ERoleType::Driver)
					{
						targetTile->SetCommandTarget(true);
						fireFighterPawn->SetFireDeckTargetTile(targetTile);
						localPlayer->NotifyReRoll();
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Firefighter Fire Deck Gun"));
						if (ensure(localPlayer) && ensure(targetTile))
						{
							localPlayer->ServerFireDeckGun(fireFighterPawn, targetTile);
						}
					}
				}
			}

			fireFighterPawn->AdjustFireFighterAP(-fireFighterPawn->GetFireDeckGunConsumption());
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
