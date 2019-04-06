// Fill out your copyright notice in the Description page of Project Settings.

#include "OptionPrompt.h"
#include "Components/Button.h"
#include "FPPlayerController.h"
#include "FireFighterPawn.h"
#include "Tile.h"
#include "GameBoard.h"

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

		if (!ensure(ThirdOption)) { return false; }
		ThirdOption->OnClicked.AddDynamic(this, &UOptionPrompt::OnThirdOption);

		return true;
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

void UOptionPrompt::PromptOperation(EOptionPromptType operation, FString promptText) {
	optionType = operation;
	if (ensure(PromptText)) {
		PromptText->SetText(FText::FromString(promptText));
	}
	SetVisibility(ESlateVisibility::Visible);
}

void UOptionPrompt::Accept()
{
	UE_LOG(LogTemp, Warning, TEXT("Accept Button has been clicked"));
	switch (optionType)
	{
	case EOptionPromptType::CommandDoor:
		CommandDoorAccept();
		this->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EOptionPromptType::CommandTile:
		CommandTileAccept();
		this->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EOptionPromptType::Dodge:
		DodgeAccept();
		this->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EOptionPromptType::FireDeckGunReRoll:
		FireDeckGunReRollAccept();
		break;
	case EOptionPromptType::FireDeckGunPositionUnComfirm:
		FireDeckGunChooseRow();
		break;
	default:
		break;
	}
	
}

void UOptionPrompt::Refuse()
{
	UE_LOG(LogTemp, Warning, TEXT("Refuse Button has been clicked"));
	switch (optionType)
	{
	case EOptionPromptType::CommandDoor:
		CommandDoorRefuse();
		this->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EOptionPromptType::CommandTile:
		CommandTileRefuse();
		this->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EOptionPromptType::Dodge:
		DodgeRefuse();
		this->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EOptionPromptType::FireDeckGunReRoll:
		FireDeckGunReRowRefuse();
		this->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EOptionPromptType::FireDeckGunPositionUnComfirm:
		FireDeckGunChooseCol();
		break;
	default:
		break;
	}
}

void UOptionPrompt::OnThirdOption()
{
	FireDeckGunPositionComfirm();
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
	if (ensure(fireFighterPawn)) {
		fireFighterPawn->AcceptDoorCommand(true);
	}
}

void UOptionPrompt::CommandTileAccept()
{
	if (ensure(fireFighterPawn)) {
		fireFighterPawn->AcceptMoveCommand(true);
	}
}

void UOptionPrompt::FireDeckGunReRollAccept()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Deck Gun Re Roll Accept"));
	if (ensure(fireFighterPawn) && ensure(fireFighterPawn->GetFireFighterRole() == ERoleType::Driver))
	{
		if (ensure(fireFighterPawn->GetFireDeckTargetTile()))
		{
			ButtonText_One->SetText(FText::FromString(FString("Row")));
			ButtonText_Two->SetText(FText::FromString(FString("Col")));
			ButtonText_Three->SetText(FText::FromString(FString("Accept")));
			ThirdOption->SetVisibility(ESlateVisibility::Visible);
			fireFighterPawn->GetFireDeckTargetTile()->SetCommandTarget(false);
			optionType = EOptionPromptType::FireDeckGunPositionUnComfirm;
			SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UOptionPrompt::FireDeckGunPositionComfirm()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Deck Gun Comfirm"));
	if (ensure(fireFighterPawn) && ensure(fireFighterPawn->GetFireDeckTargetTile()))
	{
		ATile* tempTile = fireFighterPawn->GetFireDeckTargetTile();
		AFPPlayerController* localPlayer = Cast<AFPPlayerController>(fireFighterPawn->GetController());
		if (ensure(tempTile))
		{
			tempTile->SetCommandTarget(false);
			if (ensure(localPlayer))
			{
				localPlayer->ServerFireDeckGun(fireFighterPawn, tempTile);
			}
		}
		fireFighterPawn->SetFireDeckTargetTile(nullptr);
		ThirdOption->SetVisibility(ESlateVisibility::Collapsed);
		ButtonText_One->SetText(FText::FromString(FString("Accept")));
		ButtonText_Two->SetText(FText::FromString(FString("Refuse")));
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UOptionPrompt::FireDeckGunChooseRow()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Deck Gun Row"));
	if (ensure(fireFighterPawn) && ensure(fireFighterPawn->GetFireFighterRole() == ERoleType::Driver))
	{
		ATile* tempTile = fireFighterPawn->GetFireDeckTargetTile();
		if (ensure(tempTile))
		{
			tempTile->SetCommandTarget(false);

			int32 startx;
			int32 starty;
			tempTile->GetLocation(startx, starty);
			if (startx <= 4) { startx = 1; }
			else { startx = 5; }


			// randomly choose a target fire 
			int32 randx = FMath::RandRange(startx, startx + 3);
			AGameBoard* tempGamBoard = fireFighterPawn->GetPlayingBoard();
			if (tempGamBoard)
			{
				tempTile = tempGamBoard->GetboardTiles()[randx * 8 + starty];
				if (ensure(tempTile))
				{
					fireFighterPawn->SetFireDeckTargetTile(tempTile);
					tempTile->SetCommandTarget(true);
				}
			}
		}
	}
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
	if (ensure(fireFighterPawn)) {
		fireFighterPawn->AcceptDoorCommand(false);
	}
}

void UOptionPrompt::CommandTileRefuse()
{
	if (ensure(fireFighterPawn)) {
		fireFighterPawn->AcceptMoveCommand(false);
	}
}

void UOptionPrompt::FireDeckGunReRowRefuse()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Deck Gun Refuse"));
	if (ensure(fireFighterPawn)) 
	{
		ATile* tempTile = fireFighterPawn->GetFireDeckTargetTile();
		AFPPlayerController* localPlayer = Cast<AFPPlayerController>(fireFighterPawn->GetController());
		if (ensure(tempTile))
		{
			tempTile->SetCommandTarget(false);
			if (ensure(localPlayer))
			{
				localPlayer->ServerFireDeckGun(fireFighterPawn, tempTile);
			}
		}
		fireFighterPawn->SetFireDeckTargetTile(nullptr);
	}
}

void UOptionPrompt::FireDeckGunChooseCol()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Deck Gun Column"));
	if (ensure(fireFighterPawn) && ensure(fireFighterPawn->GetFireFighterRole() == ERoleType::Driver))
	{
		ATile* tempTile = fireFighterPawn->GetFireDeckTargetTile();
		if (ensure(tempTile))
		{
			tempTile->SetCommandTarget(false);

			int32 startx;
			int32 starty;
			tempTile->GetLocation(startx, starty);
			if (starty <= 3) { starty = 1; }
			else { starty = 4; }

			// randomly choose a target fire 
			int32 randy = FMath::RandRange(starty, starty + 2);
			AGameBoard* tempGamBoard = fireFighterPawn->GetPlayingBoard();
			if (tempGamBoard)
			{
				tempTile = tempGamBoard->GetboardTiles()[startx * 8 + randy];
				if (ensure(tempTile))
				{
					fireFighterPawn->SetFireDeckTargetTile(tempTile);
					tempTile->SetCommandTarget(true);
				}
			}
		}
	}
}
