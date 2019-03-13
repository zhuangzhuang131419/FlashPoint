// Fill out your copyright notice in the Description page of Project Settings.

#include "FireFighterUI.h"
#include "UObject/ConstructorHelpers.h"
#include "GameBoard.h"

UFireFighterUI::UFireFighterUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

AGameBoard * UFireFighterUI::GetGameBoard()
{
	return gameBoard;
}

void UFireFighterUI::SetGameBoard(AGameBoard * inGameBoard)
{
	gameBoard = inGameBoard;
}

AFireFighterPawn * UFireFighterUI::GetRelatedPawn()
{
	return localPawn;
}

void UFireFighterUI::SetRelatedPawn(AFireFighterPawn * inPawn)
{
	localPawn = inPawn;
}

AFPPlayerController * UFireFighterUI::GetRelatedPlayer()
{
	return localPlayer;
}

void UFireFighterUI::SetRelatedPlayer(AFPPlayerController * inPlayer)
{
	localPlayer = inPlayer;
}

UFireFighterStatus * UFireFighterUI::GetPlayerStatusUIOf(int32 playerID)
{
	switch (playerID) {
	case 0:
		return P0Status;
		break;
	case 1:
		return P1Status;
		break;
	case 2:
		return P2Status;
		break;
	case 3:
		return P3Status;
		break;
	case 4:
		return P4Status;
		break;
	case 5:
		return P5Status;
		break;
	}

	return nullptr;
}
