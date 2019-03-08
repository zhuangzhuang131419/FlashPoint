// Fill out your copyright notice in the Description page of Project Settings.

#include "FPPlayerController.h"
#include "Victim.h"
#include "Tile.h"
#include "Wall.h"
#include "EdgeUnit.h"
#include "GameBoard.h"

AFPPlayerController::AFPPlayerController() {
	// enable mouse clicking and over events
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

}

void AFPPlayerController::ServerChopWall_Implementation(AWall * wall)
{
	wall->ChopWall();
	//ClientWallMeshUpdate(wall, !wall->IsBlocked());
}

bool AFPPlayerController::ServerChopWall_Validate(AWall * wall)
{
	return true;
}

void AFPPlayerController::ClientWallMeshUpdate_Implementation(AWall * wall, bool chopedDown)
{
	//wall->UpdateWallMesh(chopedDown);
}

bool AFPPlayerController::ClientWallMeshUpdate_Validate(AWall * wall, bool chopedDown)
{
	return true;
}

AVictim * AFPPlayerController::GetCarriedVictim()
{
	return carriedVictim;
}

void AFPPlayerController::SetCarriedVictim(AVictim * victim)
{
	carriedVictim = victim;
}

void AFPPlayerController::DropVictim()
{
	if (ensure(carriedVictim))
	{

	}
}

void AFPPlayerController::CarryVictim()
{
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	ATile* currentTile = fireFighterPawn->GetPlacedOn();
	if (currentTile->GetVictim())
	{
		carriedVictim = currentTile->GetVictim();
		currentTile->SetVictim(nullptr);
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



