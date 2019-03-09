// Fill out your copyright notice in the Description page of Project Settings.

#include "FPPlayerController.h"
#include "Victim.h"
#include "Tile.h"
#include "Wall.h"
#include "Door.h"
#include "GameBoard.h"

AFPPlayerController::AFPPlayerController() {
	// enable mouse clicking and over events
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

}

void AFPPlayerController::ServerChopWall_Implementation(AWall * wall)
{
	if (ensure(wall)) {
		wall->ChopWall();
	}
	//ClientWallMeshUpdate(wall, !wall->IsBlocked());
}

bool AFPPlayerController::ServerChopWall_Validate(AWall * wall)
{
	return true;
}

void AFPPlayerController::ServerOpenDoor_Implementation(ADoor * door)
{
	if (ensure(door)) {
		door->ChangeDoorStatus();
	}
}

bool AFPPlayerController::ServerOpenDoor_Validate(ADoor * door)
{
	return true;
}

void AFPPlayerController::ServerMovePawn_Implementation(ATile * tile, AFireFighterPawn * pawnToMove, const TArray<ATile*>& trace)
{
	if (ensure(tile)) {
		tile->PawnMoveToHere(pawnToMove, trace);
	}
}

bool AFPPlayerController::ServerMovePawn_Validate(ATile * tile, AFireFighterPawn * pawnToMove, const TArray<ATile*>& trace)
{
	return true;
}

void AFPPlayerController::ServerPlacePawn_Implementation(ATile * tile, AFireFighterPawn * pawnToPlace)
{
	if (ensure(tile)) {
		tile->PlacePawnHere(pawnToPlace);
	}
}

bool AFPPlayerController::ServerPlacePawn_Validate(ATile * tile, AFireFighterPawn * pawnToPlace)
{
	return true;
}

void AFPPlayerController::ServerExtinguishFire_Implementation(ATile * tile)
{
	if (ensure(tile)) {
		tile->ExitinguishFireOnTile();
	}
}

bool AFPPlayerController::ServerExtinguishFire_Validate(ATile * tile)
{
	return true;
}

void AFPPlayerController::ServerAdvanceFire_Implementation(AGameBoard * board)
{
	if (ensure(board)) {
		board->AdvanceFire();
	}
}

bool AFPPlayerController::ServerAdvanceFire_Validate(AGameBoard * board)
{
	return true;
}

void AFPPlayerController::DropVictim()
{
	UE_LOG(LogTemp, Warning, TEXT("Drop victim."));
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
		ATile* currentTile = fireFighterPawn->GetPlacedOn();
		if (ensure(currentTile))
		{
			fireFighterPawn->GetVictim()->victimMesh->SetVisibility(true);
			FVector VictimSocketLocation;
			switch (currentTile->GetVictims()->Num())
			{
				case 0:
					VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
					break;
				case 1:
					VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim1"));
					break;
				case 2:
					VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim3"));
					break;
				case 3:
					VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim4"));
					break;
				default:
					UE_LOG(LogTemp, Warning, TEXT("No more position"))
					VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
					break;
			}
			
			fireFighterPawn->GetVictim()->victimMesh->SetRelativeLocation(VictimSocketLocation);
			UE_LOG(LogTemp, Warning, TEXT("Before Add Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num())
			currentTile->GetVictims()->Add(fireFighterPawn->GetVictim());
			UE_LOG(LogTemp, Warning, TEXT("Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());
			fireFighterPawn->SetVictim(nullptr);
			currentTile->SetPOIStatus(EPOIStatus::Hided);
		}
	}
}

void AFPPlayerController::CarryVictim()
{
	UE_LOG(LogTemp, Warning, TEXT("Carry victim."));
	
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
		ATile* currentTile = fireFighterPawn->GetPlacedOn();
		if (ensure(currentTile))
		{
			if (ensure(currentTile->GetPOIStatus() == EPOIStatus::Revealed))
			{
				if (currentTile->GetVictims()->Num() > 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Before pop(). Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());
					AVictim* carriedVictim = currentTile->GetVictims()->Pop(true);
					UE_LOG(LogTemp, Warning, TEXT("After pop(). Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());
					fireFighterPawn->SetVictim(carriedVictim);
					carriedVictim->victimMesh->SetVisibility(false);
					currentTile->SetPOIStatus(EPOIStatus::Empty);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No victim."));
				}
			}
		}
	}
}

void AFPPlayerController::BeginPlay()
{

}

void AFPPlayerController::SetOpenDoor()
{
	CurrentOperation = EGameOperations::OpenDoor;
}

void AFPPlayerController::SetNone()
{
	CurrentOperation = EGameOperations::None;
}

void AFPPlayerController::SetPlaceFireFighter()
{
	CurrentOperation = EGameOperations::PlaceFireFighter;
}

void AFPPlayerController::SetRespawnFireFighter()
{
	CurrentOperation = EGameOperations::RespawnFireFighter;
}

void AFPPlayerController::SetMove()
{
	CurrentOperation = EGameOperations::Move;
}

void AFPPlayerController::SetExtinguishFire()
{
	CurrentOperation = EGameOperations::ExtinguishFire;
}

void AFPPlayerController::SetChopWall()
{
	CurrentOperation = EGameOperations::ChopWall;
}

void AFPPlayerController::SetCarry()
{
	CurrentOperation = EGameOperations::Carry;
}

EGameOperations AFPPlayerController::GetCurrentOperation()
{
	return CurrentOperation;
}