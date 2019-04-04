// Fill out your copyright notice in the Description page of Project Settings.

#include "Flash_PointGameModeBase.h"

void AFlash_PointGameModeBase::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
}

void AFlash_PointGameModeBase::DoSeamlessTravel(bool isSeamLess)
{
	//bUseSeamlessTravel = isSeamLess;
}
