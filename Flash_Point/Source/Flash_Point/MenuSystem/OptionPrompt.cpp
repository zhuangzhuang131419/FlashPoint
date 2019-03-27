// Fill out your copyright notice in the Description page of Project Settings.

#include "OptionPrompt.h"
#include "FPPlayerController.h"

void UOptionPrompt::AssociatePlayer(AFPPlayerController* inPlayer) {
	myOwner = inPlayer;
}


