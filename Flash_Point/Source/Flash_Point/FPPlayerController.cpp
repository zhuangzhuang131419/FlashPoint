// Fill out your copyright notice in the Description page of Project Settings.

#include "FPPlayerController.h"

AFPPlayerController::AFPPlayerController() {
	// enable mouse clicking and over events
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

}

void AFPPlayerController::BeginPlay()
{

}

void AFPPlayerController::SetOpenDoor()
{
	CurrentOperation = EGameOperations::OpenDoor;
}

EGameOperations AFPPlayerController::GetCurrentOperation()
{
	return CurrentOperation;
}



