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
			FVector VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
			fireFighterPawn->GetVictim()->victimMesh->SetRelativeLocation(VictimSocketLocation);
			currentTile->SetVictim(fireFighterPawn->GetVictim());
			fireFighterPawn->SetVictim(nullptr);
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
		UE_LOG(LogTemp, Warning, TEXT("Current Tile: %s"), *currentTile->GetName());
		if (ensure(currentTile))
		{
			if (currentTile->GetVictim())
			{
				fireFighterPawn->SetVictim(currentTile->GetVictim());
				currentTile->GetVictim()->victimMesh->SetVisibility(false);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No victim."));
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



