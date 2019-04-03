// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbulanceOperationsMenu.h"
#include "Components/Button.h"

bool UAmbulanceOperationsMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (Success)
	{
		if (!ensure(OperationButton1)) { return false; }
		OperationButton3->OnClicked.AddDynamic(this, &UAmbulanceOperationsMenu::CallAmbulance);
	}
	return false;
}

void UAmbulanceOperationsMenu::CallAmbulance()
{

}
