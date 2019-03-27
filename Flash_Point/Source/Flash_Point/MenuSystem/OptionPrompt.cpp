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
	case EOptionPromptType::Command:
		CommandAccept();
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
	case EOptionPromptType::Command:
		CommandRefuse();
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

void UOptionPrompt::CommandAccept()
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

void UOptionPrompt::CommandRefuse()
{
}
