// Fill out your copyright notice in the Description page of Project Settings.

#include "FireFighterStatus.h"

void UFireFighterStatus::AdjustAPBar(int32 cur, int32 max)
{
	float perc = (float)cur / (float)max;
	TeammateAPBar->SetPercent(FMath::Min<float>(perc, 1.0f));
}

void UFireFighterStatus::ShowPlayerName(FString name)
{
	PlayerName->SetText(FText::FromString(name));
}
