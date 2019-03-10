// Fill out your copyright notice in the Description page of Project Settings.

#include "FireFighterUI.h"
#include "GameBoard.h"

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
