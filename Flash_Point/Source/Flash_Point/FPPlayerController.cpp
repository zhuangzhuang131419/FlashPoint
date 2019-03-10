// Fill out your copyright notice in the Description page of Project Settings.

#include "FPPlayerController.h"
#include "Victim.h"
#include "Tile.h"
#include "Wall.h"
#include "Door.h"
#include "GameBoard.h"

AFPPlayerController::AFPPlayerController() {
	// enable mouse clicking and over events
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

}

void AFPPlayerController::SetGameBoard(AGameBoard * inGame)
{
	gameBoard = inGame;
}

AGameBoard * AFPPlayerController::GetGameBoard()
{
	return gameBoard;
}

int32 AFPPlayerController::GetTurnNum()
{
	return myTurnNum;
}

void AFPPlayerController::SetTurnNum(int32 turnNum)
{
	myTurnNum = turnNum;
	UE_LOG(LogTemp, Warning, TEXT("My turn num is: %d"), myTurnNum);
}

void AFPPlayerController::ServerChopWall_Implementation(AWall * wall)
{
	if (ensure(wall)) {
		wall->ChopWall();
	}
	//ClientWallMeshUpdate(wall, !wall->IsBlocked());
}

bool AFPPlayerController::ServerChopWall_Validate(AWall * wall)
{
	return true;
}

void AFPPlayerController::ServerOpenDoor_Implementation(ADoor * door)
{
	if (ensure(door)) {
		door->ChangeDoorStatus();
	}
}

bool AFPPlayerController::ServerOpenDoor_Validate(ADoor * door)
{
	return true;
}

void AFPPlayerController::ServerMovePawn_Implementation(ATile * tile, AFireFighterPawn * pawnToMove, const TArray<ATile*>& trace)
{
	if (ensure(tile)) {
		tile->PawnMoveToHere(pawnToMove, trace);
	}
}

bool AFPPlayerController::ServerMovePawn_Validate(ATile * tile, AFireFighterPawn * pawnToMove, const TArray<ATile*>& trace)
{
	return true;
}

void AFPPlayerController::ServerPlacePawn_Implementation(ATile * tile, AFireFighterPawn * pawnToPlace)
{
	if (ensure(tile)) {
		tile->PlacePawnHere(pawnToPlace);
		AGameBoard* tempBoard = tile->GetGameBoard();
		if (ensure(tempBoard)) {
			tempBoard->InitialPlacing();
		}
	}
}

bool AFPPlayerController::ServerPlacePawn_Validate(ATile * tile, AFireFighterPawn * pawnToPlace)
{
	return true;
}

void AFPPlayerController::ServerExtinguishFire_Implementation(ATile * tile)
{
	if (ensure(tile)) {
		tile->ExitinguishFireOnTile();
	}
}

bool AFPPlayerController::ServerExtinguishFire_Validate(ATile * tile)
{
	return true;
}

void AFPPlayerController::ServerAdvanceFire_Implementation(AGameBoard * board)
{
	if (ensure(board)) {
		board->AdvanceFireOnBoard();
	}
}

bool AFPPlayerController::ServerAdvanceFire_Validate(AGameBoard * board)
{
	return true;
}

void AFPPlayerController::ServerAdvancePOI_Implementation(AGameBoard * board)
{
	if (ensure(board)) {
		board->AdvancePOIOnBoard();
	}
}

bool AFPPlayerController::ServerAdvancePOI_Validate(AGameBoard * board)
{
	return true;
}

void AFPPlayerController::ServerDrop_Implementation(AFireFighterPawn * fireFighterPawn)
{
	ATile* currentTile = fireFighterPawn->GetPlacedOn();
	if (ensure(currentTile))
	{
		AVictim* tempVictim = fireFighterPawn->GetVictim();
		if (!tempVictim) return;
		if (currentTile->IsOutside())
		{
			tempVictim->Destroy();
			UE_LOG(LogTemp, Warning, TEXT("Outside"));
			currentTile->SetPOIStatus(EPOIStatus::Empty);
			currentTile->GetGameBoard()->SetVictimSavedNum(
				currentTile->GetGameBoard()->victimSavedNum + 1
			);
			UE_LOG(LogTemp, Warning, TEXT("Current saved victim: %d"), currentTile->GetGameBoard()->victimSavedNum);
			currentTile->GetGameBoard()->SetCurrentPOI(
				currentTile->GetGameBoard()->currentPOI - 1
			);
			UE_LOG(LogTemp, Warning, TEXT("Current saved victim: %d"), currentTile->GetGameBoard()->currentPOI);
		}
		else {
			tempVictim->victimMesh->SetVisibility(true);
			FVector VictimSocketLocation;
			switch (currentTile->GetVictims()->Num())
			{
			case 0:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
				break;
			case 1:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim1"));
				break;
			case 2:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim3"));
				break;
			case 3:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim4"));
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("No more position"))
					VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
				break;
			}

			tempVictim->victimMesh->SetRelativeLocation(VictimSocketLocation);
			tempVictim->SetVictimLoc(tempVictim->GetActorLocation());

			UE_LOG(LogTemp, Warning, TEXT("Before Add Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());
			currentTile->GetVictims()->Add(fireFighterPawn->GetVictim());
			UE_LOG(LogTemp, Warning, TEXT("Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());
			
			currentTile->SetPOIStatus(EPOIStatus::Revealed);
		}
		fireFighterPawn->SetVictim(nullptr);
		tempVictim->SetIsCarried(false);
	}
}

bool AFPPlayerController::ServerDrop_Validate(AFireFighterPawn * fireFighterPawn)
{
	return true;
}

void AFPPlayerController::ServerCarryVictim_Implementation(AFireFighterPawn * fireFighterPawn)
{
	ATile* currentTile = fireFighterPawn->GetPlacedOn();
	if (ensure(currentTile))
	{
		if (currentTile->GetPOIStatus() == EPOIStatus::Revealed)
		{
			if (currentTile->GetVictims()->Num() > 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Before pop(). Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());
				AVictim* carriedVictim = currentTile->GetVictims()->Pop(true);
				if (!ensure(carriedVictim)) return;
				carriedVictim->SetIsCarried(true);
				UE_LOG(LogTemp, Warning, TEXT("After pop(). Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());
				fireFighterPawn->SetVictim(carriedVictim);
				carriedVictim->victimMesh->SetVisibility(false);
				currentTile->SetPOIStatus(EPOIStatus::Empty);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No victim."));
			}
		}
	}
}

bool AFPPlayerController::ServerCarryVictim_Validate(AFireFighterPawn * fireFighterPawn)
{
	return true;
}

void AFPPlayerController::ServerGetFireFighterID_Implementation(AFireFighterPawn * fireFighterPawn, AGameBoard * inGameBoard)
{
	if (ensure(fireFighterPawn) && ensure(inGameBoard)) {
		fireFighterPawn->SetFireFighterID(inGameBoard->JoinBoard());
	}
}

bool AFPPlayerController::ServerGetFireFighterID_Validate(AFireFighterPawn * fireFighterPawn, AGameBoard * inGameBoard)
{
	return true;
}

void AFPPlayerController::ServerEndTurn_Implementation(AGameBoard * inGameBoard)
{
	if (ensure(inGameBoard)) {

	}
}

bool AFPPlayerController::ServerEndTurn_Validate(AGameBoard * inGameBoard)
{
	return true;
}

void AFPPlayerController::DropVictim()
{
	UE_LOG(LogTemp, Warning, TEXT("Drop victim."));
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
		// for client only, do the victim position visual settings
		if (!HasAuthority()) {
			ATile* currentTile = fireFighterPawn->GetPlacedOn();
			if (!ensure(currentTile)) return;
			FVector VictimSocketLocation;
			switch (currentTile->GetVictims()->Num())
			{
			case 0:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
				break;
			case 1:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim1"));
				break;
			case 2:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim3"));
				break;
			case 3:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim4"));
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("No more position"));
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
				break;
			}
			AVictim* tempVictim = fireFighterPawn->GetVictim();
			if (!ensure(tempVictim)) return;
			tempVictim->victimMesh->SetRelativeLocation(VictimSocketLocation);
		}
		ServerDrop(fireFighterPawn);
	}
}

void AFPPlayerController::CarryVictim()
{
	UE_LOG(LogTemp, Warning, TEXT("Carry victim."));
	
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
		ServerCarryVictim(fireFighterPawn);
	}
}

void AFPPlayerController::FindGameBoard()
{
	// ensure there is a world
	UWorld* world = GetWorld();
	if (ensure(world)) {
		TArray<AActor*> allGameBoard;
		UGameplayStatics::GetAllActorsOfClass(world, AGameBoard::StaticClass(), allGameBoard);
		// only assign correct game board if there is one found
		if (allGameBoard.Num() > 0) {
			UE_LOG(LogTemp, Warning, TEXT("Player found gameboard"));
			gameBoard = Cast<AGameBoard>(allGameBoard[0]);
		}
	}
}

void AFPPlayerController::MakeBasicFireFighterUI()
{
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn) && ensure(gameBoard))
	{
		if (ensure(BasicFireFighterClass)) {
			inGameUI = CreateWidget<UFireFighterUI>(this, BasicFireFighterClass);
			if (ensure(inGameUI)) {
				inGameUI->SetGameBoard(gameBoard);
				inGameUI->SetRelatedPlayer(this);
				inGameUI->SetRelatedPawn(fireFighterPawn);
				inGameUI->AddToViewport();
			}
		}
	}
}

void AFPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Player GIAO!"));
	FindGameBoard();
	// TODO on later version make different UI with regard of different game
	MakeBasicFireFighterUI();
}

void AFPPlayerController::SetOpenDoor()
{
	CurrentOperation = EGameOperations::OpenDoor;
}

void AFPPlayerController::SetNone()
{
	CurrentOperation = EGameOperations::None;
}

void AFPPlayerController::SetPlaceFireFighter()
{
	CurrentOperation = EGameOperations::PlaceFireFighter;
}

void AFPPlayerController::SetRespawnFireFighter()
{
	CurrentOperation = EGameOperations::RespawnFireFighter;
}

void AFPPlayerController::SetMove()
{
	CurrentOperation = EGameOperations::Move;
}

void AFPPlayerController::SetExtinguishFire()
{
	CurrentOperation = EGameOperations::ExtinguishFire;
}

void AFPPlayerController::SetChopWall()
{
	CurrentOperation = EGameOperations::ChopWall;
}

void AFPPlayerController::SetCarry()
{
	CurrentOperation = EGameOperations::Carry;
}

EGameOperations AFPPlayerController::GetCurrentOperation()
{
	return CurrentOperation;
}