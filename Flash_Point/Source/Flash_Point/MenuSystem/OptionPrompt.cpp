// Fill out your copyright notice in the Description page of Project Settings.

#include "OptionPrompt.h"
#include "Components/Button.h"
#include "FPPlayerController.h"
#include "FireFighterPawn.h"

bool UOptionPrompt::Initialize()
{
	bool Success = Super::Initialize();
	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bind Function"));
		if (!ensure(AcceptButton)) { return false; }
		AcceptButton->OnClicked.AddDynamic(this, &UOptionPrompt::Accept);

		if (!ensure(RefuseButton)) { return false; }
		RefuseButton->OnClicked.AddDynamic(this, &UOptionPrompt::Refuse);
	}
	return false;
}

void UOptionPrompt::AssociatePlayer(AFPPlayerController* inPlayer) {
	myOwner = inPlayer;
	if (ensure(myOwner))
	{
		fireFighterPawn = Cast<AFireFighterPawn>(myOwner->GetPawn());
	}
}

void UOptionPrompt::Accept()
{
	UE_LOG(LogTemp, Warning, TEXT("Accept"));
	switch (optionType)
	{
	case EOptionPromptType::CommandDoor:
		CommandDoorAccept();
		break;
	case EOptionPromptType::CommandTile:
		CommandTileAccept();
		break;
	case EOptionPromptType::Dodge:
		DodgeAccept();
		break;
	default:
		break;
	}
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UOptionPrompt::Refuse()
{
	UE_LOG(LogTemp, Warning, TEXT("Refuse"));
	switch (optionType)
	{
	case EOptionPromptType::CommandDoor:
		CommandDoorRefuse();
		break;
	case EOptionPromptType::CommandTile:
		CommandTileRefuse();
		break;
	case EOptionPromptType::Dodge:
		DodgeRefuse();
		break;
	default:
		break;
	}
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UOptionPrompt::DodgeAccept()
{
	UE_LOG(LogTemp, Warning, TEXT("Dodge Accept"));
	if (ensure(myOwner))
	{
		myOwner->SetDodge();
	}
}

void UOptionPrompt::CommandDoorAccept()
{
	// if player accepts to open door, do open the door
	
}

void UOptionPrompt::CommandTileAccept()
{
}

void UOptionPrompt::DodgeRefuse()
{
	UE_LOG(LogTemp, Warning, TEXT("Dodge Refuse"));
	if (ensure(fireFighterPawn))
	{
		fireFighterPawn->KnockDown();
	}
}

void UOptionPrompt::CommandDoorRefuse()
{
}

void UOptionPrompt::CommandTileRefuse()
{
}
