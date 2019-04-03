// Fill out your copyright notice in the Description page of Project Settings.

#include "FireEngineOperationsMenu.h"
#include "Components/Button.h"
#include "FireFighterPawn.h"
#include "Ambulance.h"
#include "Gameboard.h"

bool UFireEngineOperationsMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (Success)
	{
		if (!ensure(OperationButton3)) { return false; }
		OperationButton3->OnClicked.AddDynamic(this, &UFireEngineOperationsMenu::FireDeckGun);
	}
	return true;
}

void UFireEngineOperationsMenu::FireDeckGun()
{
}
