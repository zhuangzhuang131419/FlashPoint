// Fill out your copyright notice in the Description page of Project Settings.

#include "GameBoard.h"
#include "POI.h"
#include "Victim.h"
#include "Hazmat.h"
#include "FireEngine.h"
#include "Ambulance.h"
#include "FlashPointGameInstance.h"


// Sets default values
AGameBoard::AGameBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// family rule default set up
	/*POIInitializeLocation.Add(FLocation(5, 2));
	POIInitializeLocation.Add(FLocation(8, 5));
	POIInitializeLocation.Add(FLocation(1, 5));*/

	/*fireInitializeLocation.Add(FLocation(7, 2));
	fireInitializeLocation.Add(FLocation(6, 2));
	fireInitializeLocation.Add(FLocation(7, 3));
	fireInitializeLocation.Add(FLocation(6, 3));
	fireInitializeLocation.Add(FLocation(5, 3));
	fireInitializeLocation.Add(FLocation(4, 3));
	fireInitializeLocation.Add(FLocation(5, 4));
	fireInitializeLocation.Add(FLocation(3, 5));
	fireInitializeLocation.Add(FLocation(2, 5));
	fireInitializeLocation.Add(FLocation(3, 6));*/

}

AFPPlayerController * AGameBoard::GetCurrentPlayer()
{
	return currentPlayer;
}

void AGameBoard::SetCurrentPlayer(AFPPlayerController * current)
{
	currentPlayer = current;
}

void AGameBoard::SetCurrentGameHealth(int32 currentHealth)
{
	health = currentHealth;
	// if we are the server, actively check the health
	if (HasAuthority() && health <= 0) {
		if (ensure(localPlayer)) {
			localPlayer->NotifyGameOver(false);
		}
	}
}

int32 AGameBoard::GetCurrentTurn()
{
	return currentTurn;
}

bool AGameBoard::IsEngineTile(ATile * inTile)
{
	if (inTile == engineLocA || inTile == engineLocB) {
		return true;
	}
	else {
		return false;
	}
}

bool AGameBoard::IsAmbulanceTile(ATile * inTile)
{
	if (inTile == ambulanceLocA || inTile == ambulanceLocB) {
		return true;
	}
	else {
		return false;
	}
}

void AGameBoard::ClearAllTile()
{
	for (ATile* t : boardTiles) {
		if (ensure(t)) {
			t->ResetTile();
		}
	}
}

void AGameBoard::AdvanceFireOnBoard()
{
	UE_LOG(LogTemp, Warning, TEXT("Advance fire."));
	int32 randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
	while (boardTiles[randomPosition]->IsOutside())
	{
		randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
	}

	if (boardTiles[randomPosition]->GetFireStatus() == EFireStatus::Clear)
	{
		UE_LOG(LogTemp, Warning, TEXT("Advance smoke at.%s"), *boardTiles[randomPosition]->GetName());
		boardTiles[randomPosition]->SetFireStatus(EFireStatus::Smoke);
		boardTiles[randomPosition]->GetSmokeEffect()->Activate();
	}
	else if (boardTiles[randomPosition]->GetFireStatus() == EFireStatus::Smoke)
	{
		UE_LOG(LogTemp, Warning, TEXT("Advance fire at.%s"), *boardTiles[randomPosition]->GetName());
		boardTiles[randomPosition]->SetFireStatus(EFireStatus::Fire);
		boardTiles[randomPosition]->GetFireEffect()->Activate();
		boardTiles[randomPosition]->GetSmokeEffect()->Deactivate();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Advance blast at.%s"), *boardTiles[randomPosition]->GetName());
		boardTiles[randomPosition]->GetBlastEffect()->DeactivateSystem();
		boardTiles[randomPosition]->GetBlastEffect()->ActivateSystem();
		boardTiles[randomPosition]->AdvanceExplosion();
	}

	if (gameModeType == EGameType::Experienced)
	{
		// solve flare up
		if (boardTiles[randomPosition]->IsHotSpot() && health > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Advance again"));
			AdvanceFireOnBoard();
		}
	}
}

void AGameBoard::AdvanceFire()
{
	if (HasAuthority()) {
		AdvanceFireOnBoard();
	}
	else {
		if (ensure(localPlayer)) {
			localPlayer->ServerAdvanceFire(this);
		}
	}
}

void AGameBoard::AdvancePOIOnBoard()
{
	while (currentPOI < maxPOI && (totalVictimNum > 0 || falseAlarmNum > 0))
	{
		int32 randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
		while (boardTiles[randomPosition]->GetPOIStatus() != EPOIStatus::Empty
			|| boardTiles[randomPosition]->IsOutside()
			|| boardTiles[randomPosition]->GetFireStatus() == EFireStatus::Fire
			|| boardTiles[randomPosition]->GetPlacedFireFighters()->Num() > 0)
		{
			randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
		}

		boardTiles[randomPosition]->AdvancePOI();
		UE_LOG(LogTemp, Warning, TEXT("advance POI."));
	}
}

void AGameBoard::AdvancePOI()
{
	if (HasAuthority()) {
		AdvancePOIOnBoard();
	}
	else {
		if (ensure(localPlayer)) {
			localPlayer->ServerAdvancePOI(this);
		}
	}
}

void AGameBoard::FlashOverOnBoard()
{
	UE_LOG(LogTemp, Warning, TEXT("Flash Over"));
	bool isDone;
	do
	{
		isDone = true;
		for (ATile* tile : boardTiles)
		{
			if (!tile->Flashover())
			{
				isDone = false;
			}
		}
	} while (!isDone);
}

void AGameBoard::flashover()
{	
	if (HasAuthority()) {
		FlashOverOnBoard();
	}
	else {
		if (ensure(localPlayer)) {
			localPlayer->ServerFlashOver(this);
		}
	}
}

void AGameBoard::ResolveKnockDownOnBoard()
{
	UE_LOG(LogTemp, Warning, TEXT("Handling knockdown"));
	for (ATile* tile : boardTiles)
	{
		// Any firefighters in a space with fire are knocked down
		if (tile->GetPlacedFireFighters()->Num() > 0 && tile->GetFireStatus() == EFireStatus::Fire)
		{
			UE_LOG(LogTemp, Warning, TEXT("number: %d"), tile->GetPlacedFireFighters()->Num());

			for (AFireFighterPawn* fireFighterPawn : *(tile->GetPlacedFireFighters()))
			{
				AFPPlayerController* localController = Cast<AFPPlayerController>(fireFighterPawn->GetController());
				if (ensure(localController) && ensure(fireFighterPawn))
				{
					if (fireFighterPawn->CheckIsVicinty(veteranLoc))
					{
						fireFighterPawn->SetDodgeAbility(true);
					}
					else
					{
						fireFighterPawn->SetDodgeAbility(false);
					}

					if (fireFighterPawn->GetCanDodge())
					{
						// Server
						if (GetWorld()->GetFirstPlayerController() == localController)
						{
							localController->NotifyPlayerDodge();
						}
						else
						{
							fireFighterPawn->FlipServerDodgeFlag();
						}
					}
					else
					{
						fireFighterPawn->KnockDown();
					}
				}
			}
		}

		// Any victims or POI in a space with fire are lost
		if (tile->GetPOIOnTile())
		{
			if (tile->GetFireStatus() == EFireStatus::Fire)
			{
				if (tile->GetPOIOnTile()->isAlarm) {
					tile->GetGameBoard()->SetVictimLostNum(tile->GetGameBoard()->victimLostNum + 1);
				}
				tile->GetPOIOnTile()->Destroy();
				tile->SetPOIStatus(EPOIStatus::Empty);
				tile->SetPOIOnTile(nullptr);
				tile->GetGameBoard()->SetCurrentPOI(tile->GetGameBoard()->currentPOI - 1);
			}
		}

		if (tile->GetVictims()->Num() > 0)
		{
			if (tile->GetFireStatus() == EFireStatus::Fire)
			{
				while (tile->GetVictims()->Num() > 0)
				{
					AVictim* tempVictim = tile->GetVictims()->Pop();
					if (ensure(tempVictim))
					{
						tempVictim->Destroy();
						tile->GetGameBoard()->SetCurrentPOI(tile->GetGameBoard()->currentPOI - 1);
						tile->GetGameBoard()->SetVictimLostNum(tile->GetGameBoard()->victimLostNum + 1);
					}
				}
				tile->removeVictims();
			}
		}

		// Remove any fire markers that were placed outside of the building
		if (tile->IsOutside())
		{
			tile->SetFireStatus(EFireStatus::Clear);
			tile->GetFireEffect()->Deactivate();
			tile->GetSmokeEffect()->Deactivate();
		}

		// actively check if the lost victims is too much
		if (HasAuthority() && victimLostNum >= maxLostVictim) {
			if (!ensure(localPlayer)) return;
			localPlayer->NotifyGameOver(false);
		}
	}
}

void AGameBoard::resolveKnockDown()
{
	if (HasAuthority())
	{
		ResolveKnockDownOnBoard();
	}
	else
	{
		if (ensure(localPlayer))
		{
			localPlayer->ServerSolveKnockDown(this);
		}
	}
}

void AGameBoard::ResolveHazmatExplosionOnBoard()
{
	UE_LOG(LogTemp, Warning, TEXT("Handling hazmatExplosion"));
	for (ATile* tile : boardTiles)
	{
		// Any firefighters in a space with fire are knocked down
		if (tile->GetHazmat() != nullptr && tile->GetFireStatus() == EFireStatus::Fire)
		{
			tile->AdvanceExplosion();
			tile->SetIsHotSpot(true);
			tile->GetHotSpotEffect()->Activate();

			// remove the hazmat
			tile->GetHazmat()->Destroy();
			tile->SetHazmatOnTile(nullptr);
		}
	}
}

void AGameBoard::resolveHazmatExplosions()
{

	if (HasAuthority())
	{
		ResolveHazmatExplosionOnBoard();
	}
	else
	{
		if (ensure(localPlayer))
		{
			localPlayer->ServerSolveHazmatExplosions(this);
		}
	}
}

void AGameBoard::ForceGetOutAmbulance()
{
	UE_LOG(LogTemp, Warning, TEXT("Force get out of ambulance."));
	if (ensure(ambulance) && ambulance->GetPassengers()->Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current ambulance has %d firefighters."), ambulance->GetPassengers()->Num());
		if (ensure(localPlayer) && ensure(ambulanceLocA))
		{
			localPlayer->ServerAllPassengersGetOutAmbulance(ambulanceLocA, ambulance);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No passenger is in the ambulance"));
	}
}

void AGameBoard::ForceGetOutFireEngine()
{
	UE_LOG(LogTemp, Warning, TEXT("Force get out of fire engine."));
	if (ensure(fireEngine) && fireEngine->GetPassengers()->Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current fire engine has %d firefighters."), fireEngine->GetPassengers()->Num());
		if (ensure(localPlayer) && ensure(engineLocA))
		{
			localPlayer->ServerAllPassengersGetOutFireEngine(engineLocA, fireEngine);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No passenger is in the fire engine"));
	}
}

// return true if end the game, otherwise return false
void AGameBoard::endTurnRelatedOperations()
{
	// assume family mode
	if (gameModeType == EGameType::Family)
	{
		AdvanceFire();
		flashover();
		resolveKnockDown();
		AdvancePOI();
	}
	else if (gameModeType == EGameType::Experienced)
	{
		ForceGetOutAmbulance();
		ForceGetOutFireEngine();
		AdvanceFire();
		flashover();
		resolveHazmatExplosions();
		resolveKnockDown();
		AdvancePOI();
	}
	
}

int32 AGameBoard::JoinBoard()
{
	if (HasAuthority()) {
		joinedPlayerNum++;
		return joinedPlayerNum - 1;
	}
	return -1;
}

void AGameBoard::LeaveBoard(AFireFighterPawn * leavePawn)
{
	if (HasAuthority()) {
		localPlayer->HostEndGame();
		localPlayer->ClientTravel("/Game/maps/MainMenu", ETravelType::TRAVEL_Absolute);
		localPlayer->LeaveGameSession();
	}
	else {
		localPlayer->ServerExitFromBoard(this, leavePawn);
	}
}

void AGameBoard::TurnSwitch()
{
	if (HasAuthority()) {
		currentTurn = (currentTurn + 1) % joinedPlayerNum;
		if (ensure(localPlayer)) {
			if (localPlayer->GetTurnNum() == currentTurn) {
				// special notify for player on server
				localPlayer->NotifyPlayerTurn();
				// TODO check if the pawn's location is same as fire engine
			}
		}
	}
}

void AGameBoard::InitialPlacing()
{
	if (HasAuthority()) {
		placedNum++;
		if (placedNum == joinedPlayerNum) {
			// if the game mode is not family mode, do not do turn switch yet
			if (gameModeType == EGameType::Family) {
				TurnSwitch();
			}
			else {
				// notify the host player to do initial fire engine placement
				if (ensure(localPlayer)) {
					localPlayer->PromtPlacingVehicle("Place Fire Engine");
					localPlayer->SetCurrentOperation(EGameOperations::PlaceFireEngine);
				}
			}
		}
	}
}

void AGameBoard::InitializeDefaultBoard()
{
	// This is only done on the server
	if (!HasAuthority()) return;
	// Initialize the TArray
	if (ensure(TileClass && RoadClass)) {
		ATile* tempTile = nullptr;
		// Generate all game board tiles
		for (int32 i = 0; i < boardWidth; i++) {
			for (int32 j = 0; j < boardLength; j++) {
				FVector tempLoc = FVector(TILE_SIZE * j, TILE_SIZE * i, 0) + GetActorLocation();
				if (i == 0 || j == 0 || i == boardWidth - 1 || j == boardLength - 1) {
					// spawn road at the out most layer of the game board
					tempTile = GetWorld()->SpawnActor<ATile>(
						RoadClass,
						tempLoc,
						GetActorRotation()
						);
				}
				else {
					tempTile = GetWorld()->SpawnActor<ATile>(
						TileClass,
						tempLoc,
						GetActorRotation()
						);
				}
				if (ensure(tempTile)) {
					// initialize the tile's location
					tempTile->SetLocation(i, j);
					// Separate the board into 4 quadrants
					if (i < boardWidth / 2 && j < boardLength / 2) {
						tempTile->SetQuadrant(0);
					}
					else if (i >= boardWidth / 2 && j >= boardLength / 2) {
						tempTile->SetQuadrant(2);
					}
					else if (i < boardWidth / 2) {
						tempTile->SetQuadrant(1);
					}
					else {
						tempTile->SetQuadrant(3);
					}
					// add the tile to board tiles in order to store it
					tempTile->LinkGameBoard(this);
					boardTiles.Add(tempTile);
				}
			}
		}

		// Generate all default edges
		AEdgeUnit* tempEdge = nullptr;
		tempTile = nullptr;
		ATile* tempNeighbour = nullptr;
		// Generate all front edges and bind them
		for (int32 i = 0; i < boardWidth; i++) {
			for (int32 j = 0; j < boardLength - 1; j++) {
				tempTile = boardTiles[i * boardLength + j];
				if (ensure(tempTile)) {
					if ((j == 0 || j == boardLength - 2) && (i > 0 && i < boardWidth - 1)) {
						// for boundary edges, generate walls
						tempEdge = tempTile->BuildEdgeFront(1);
						surroundingEdges.Add(tempEdge);
					}
					else {
						// otherwise build a default empty edge
						tempEdge = tempTile->BuildEdgeFront(0);
					}
					// connecting the edge with both of the neighbours
					if (ensure(tempEdge)) {
						tempEdge->BindBoard(this);
						tempTile->BindFrontEdge(tempEdge);
						tempNeighbour = boardTiles[i * boardLength + j + 1];
						if (ensure(tempNeighbour)) {
							tempNeighbour->BindBackEdge(tempEdge);
						}
					}
				}
			}
		}

		// Generate all right edges and bind them
		tempEdge = nullptr;
		tempTile = nullptr;
		tempNeighbour = nullptr;
		for (int32 i = 0; i < boardWidth - 1; i++) {
			for (int32 j = 0; j < boardLength; j++) {
				tempTile = boardTiles[i * boardLength + j];
				if (ensure(tempTile)) {
					if ((i == 0 || i == boardWidth - 2) && (j > 0 && j < boardLength - 1)) {
						// for boundary edges, generate walls
						tempEdge = tempTile->BuildEdgeRight(1);
						surroundingEdges.Add(tempEdge);
					}
					else {
						// otherwise build a default empty edge
						tempEdge = tempTile->BuildEdgeRight(0);
					}
					// connecting the edge with both of the neighbours
					if (ensure(tempEdge)) {
						tempEdge->BindBoard(this);
						tempTile->BindRightEdge(tempEdge);
						tempNeighbour = boardTiles[(i + 1) * boardLength + j];
						if (ensure(tempNeighbour)) {
							tempNeighbour->BindLeftEdge(tempEdge);
						}
					}
				}
			}
		}
	}
	// figure out the game mode from the server's game instance
	UFlashPointGameInstance* gameInst = Cast<UFlashPointGameInstance>(GetGameInstance());
	if (ensure(gameInst)) {
		gameModeType = gameInst->GetGameType();
	}
}

void AGameBoard::GenerateSpecified(FSpawnIndicator indicator)
{
	if (!HasAuthority()) return;
	// store the indicator for save and load
	storedIndicator = indicator;
	ATile* tempTile = nullptr;
	ATile* tempNeighbour = nullptr;
	AEdgeUnit* tempEdge = nullptr;
	// first generate all specified doors on boundaries
	for (int32 i = 0; i < indicator.doorLoc.Num(); i++) {
		// in case for multiple doors on a single side use mod to separate each case
		if (i % 4 == 0 && indicator.doorLoc[i] > -1) {
			// here is a door on the frontest boundary
			tempTile = boardTiles[indicator.doorLoc[i]];
			if (ensure(tempTile)) {
				// generate the door
				tempEdge = tempTile->BuildEdgeFront(2);
				if (ensure(tempEdge)) {
					tempEdge->BindBoard(this);
					// open the door since it is on boundaries
					tempEdge->OnOpenDoor();
					// bind the door
					tempTile->BindFrontEdge(tempEdge);
					tempNeighbour = boardTiles[indicator.doorLoc[i] + 1];
					if (ensure(tempNeighbour)) {
						tempNeighbour->BindBackEdge(tempEdge);
					}
					// record the edge for saving purpose
					specialEdges.Add(tempEdge);
				}
			}
		}
		if (i % 4 == 1 && indicator.doorLoc[i] > -1) {
			// here is a door on the backest boundary
			tempTile = boardTiles[indicator.doorLoc[i]];
			if (ensure(tempTile)) {
				// generate the door and open it
				tempEdge = tempTile->BuildEdgeFront(2);
				if (ensure(tempEdge)) {
					tempEdge->BindBoard(this);
					// open the door since it is on boundaries
					tempEdge->OnOpenDoor();
					// bind the door
					tempTile->BindFrontEdge(tempEdge);
					tempNeighbour = boardTiles[indicator.doorLoc[i] + 1];
					if (ensure(tempNeighbour)) {
						tempNeighbour->BindBackEdge(tempEdge);
					}
					// record the edge for saving purpose
					specialEdges.Add(tempEdge);
				}
			}
		}
		if (i % 4 == 2 && indicator.doorLoc[i] > -1) {
			// here is a door on the leftest boundary
			tempTile = boardTiles[indicator.doorLoc[i]];
			if (ensure(tempTile)) {
				// generate the door
				tempEdge = tempTile->BuildEdgeRight(2);
				if (ensure(tempEdge)) {
					tempEdge->BindBoard(this);
					// open the door since it is on boundaries
					tempEdge->OnOpenDoor();
					// bind the door
					tempTile->BindRightEdge(tempEdge);
					tempNeighbour = boardTiles[indicator.doorLoc[i] + boardLength];
					if (ensure(tempNeighbour)) {
						tempNeighbour->BindLeftEdge(tempEdge);
					}
					// record the edge for saving purpose
					specialEdges.Add(tempEdge);
				}
			}
		}
		if (i % 4 == 3 && indicator.doorLoc[i] > -1) {
			// here is a door on the rightest boundary
			tempTile = boardTiles[indicator.doorLoc[i]];
			if (ensure(tempTile)) {
				// generate the door
				tempEdge = tempTile->BuildEdgeRight(2);
				if (ensure(tempEdge)) {
					tempEdge->BindBoard(this);
					// open the door since it is on boundaries
					tempEdge->OnOpenDoor();
					// bind the door
					tempTile->BindRightEdge(tempEdge);
					tempNeighbour = boardTiles[indicator.doorLoc[i] + boardLength];
					if (ensure(tempNeighbour)) {
						tempNeighbour->BindLeftEdge(tempEdge);
					}
					// record the edge for saving purpose
					specialEdges.Add(tempEdge);
				}
			}
		}
	}

	// Now set up specific tiles for fire engine park and ambulance park
	ATile* anotherTile = nullptr;
	tempTile = nullptr;
	for (int32 i = 0; i < indicator.engineParkLoc.Num(); i++) {
		if (ensure(indicator.engineParkLoc[i] > -1) && ensure(indicator.engineParkLoc[i] < boardWidth * boardLength)) {
			// set up for the first tile on the specified location
			tempTile = boardTiles[indicator.engineParkLoc[i]];
			if (ensure(tempTile))
			{
				tempTile->SetTileType(ETileType::FireEnginePark);
				engineTiles.Add(tempTile);
				AEdgeUnit* rightEdge = tempTile->GetRight();
				AEdgeUnit* frontEdge = tempTile->GetFront();
				AEdgeUnit* leftEdge = tempTile->GetLeft();
				AEdgeUnit* backEdge = tempTile->GetBack();
				
				if (!rightEdge || !leftEdge) 
				{ 
					// Get the second tile on front
					if (ensure(frontEdge))
					{
						anotherTile = frontEdge->GetOtherNeighbour(tempTile);
						if (ensure(anotherTile))
						{
							anotherTile->SetTileType(ETileType::FireEnginePark);
							engineTiles.Add(anotherTile);
							tempTile->SetAdjacentParkTile(anotherTile);
							anotherTile->SetAdjacentParkTile(tempTile);
						}
					}
				}
				else if (!frontEdge || !backEdge) 
				{ 
					// Get the second tile on right
					if (ensure(rightEdge))
					{
						anotherTile = rightEdge->GetOtherNeighbour(tempTile);
						if (ensure(anotherTile))
						{
							anotherTile->SetTileType(ETileType::FireEnginePark);
							engineTiles.Add(anotherTile);
							tempTile->SetAdjacentParkTile(anotherTile);
							anotherTile->SetAdjacentParkTile(tempTile);
						}
					}
				}
			}
		}
	}
	if (ensure(tempTile) && ensure(anotherTile))
	{
		if (!(gameModeType == EGameType::Family)) {
			tempTile->SpawnFireEngine();
			engineLocA = tempTile;
			engineLocB = anotherTile;
			if (ensure(fireEngine)) {
				fireEngine->ShowEnginePlaced(false);
			}
		}
	}

	
	anotherTile = nullptr;
	tempTile = nullptr;
	for (int32 i = 0; i < indicator.ambulanceParkLoc.Num(); i++) {
		if (ensure(indicator.ambulanceParkLoc[i] > -1) && ensure(indicator.ambulanceParkLoc[i] < boardWidth * boardLength)) {
			// set up for the first tile on the specified location
			tempTile = boardTiles[indicator.ambulanceParkLoc[i]];
			if (ensure(tempTile))
			{
				tempTile->SetTileType(ETileType::AmbulancePark);
				ambulanceTiles.Add(tempTile);
				AEdgeUnit* rightEdge = tempTile->GetRight();
				AEdgeUnit* frontEdge = tempTile->GetFront();
				AEdgeUnit* leftEdge = tempTile->GetLeft();
				AEdgeUnit* backEdge = tempTile->GetBack();

				if (!rightEdge || !leftEdge)
				{
					// Get the second tile on front
					if (ensure(frontEdge))
					{
						anotherTile = frontEdge->GetOtherNeighbour(tempTile);
						if (ensure(anotherTile))
						{
							anotherTile->SetTileType(ETileType::AmbulancePark);
							ambulanceTiles.Add(anotherTile);
							tempTile->SetAdjacentParkTile(anotherTile);
							anotherTile->SetAdjacentParkTile(tempTile);
						}
					}
				}
				else if (!frontEdge || !backEdge)
				{
					// Get the second tile on right
					if (ensure(rightEdge))
					{
						anotherTile = rightEdge->GetOtherNeighbour(tempTile);
						if (ensure(anotherTile))
						{
							anotherTile->SetTileType(ETileType::AmbulancePark);
							ambulanceTiles.Add(anotherTile);
							tempTile->SetAdjacentParkTile(anotherTile);
							anotherTile->SetAdjacentParkTile(tempTile);
						}
					}
				}
			}
		}
	}
	if (ensure(tempTile) && ensure(anotherTile))
	{
		if (!(gameModeType == EGameType::Family)) {
			tempTile->SpawnAmbulance();
			ambulanceLocA = tempTile;
			ambulanceLocB = anotherTile;
			if (ensure(ambulance)) {
				ambulance->ShowAmbulancePlaced(false);
			}
		}
	}


	// Now setup walls on indicated location's right side
	tempTile = nullptr;
	tempNeighbour = nullptr;
	tempEdge = nullptr;
	for (int32 i = 0; i < indicator.wallRight.Num(); i++) {
		if (indicator.wallRight[i] > -1) {
			tempTile = boardTiles[indicator.wallRight[i]];
			if (ensure(tempTile)) {
				// build the wall
				tempEdge = tempTile->BuildEdgeRight(1);
				if (ensure(tempEdge)) {
					tempEdge->BindBoard(this);
					// bind the wall
					tempTile->BindRightEdge(tempEdge);
					tempNeighbour = boardTiles[indicator.wallRight[i] + 8];
					if (ensure(tempNeighbour)) {
						tempNeighbour->BindLeftEdge(tempEdge);
					}
					// record the edge for saving purpose
					specialEdges.Add(tempEdge);
				}
			}
		}
	}

	// Now setup walls on indicated location's front side
	tempTile = nullptr;
	tempNeighbour = nullptr;
	tempEdge = nullptr;
	for (int32 i = 0; i < indicator.wallFront.Num(); i++) {
		if (indicator.wallFront[i] > -1) {
			tempTile = boardTiles[indicator.wallFront[i]];
			if (ensure(tempTile)) {
				// build the wall
				tempEdge = tempTile->BuildEdgeFront(1);
				if (ensure(tempEdge)) {
					tempEdge->BindBoard(this);
					// bind the wall
					tempTile->BindFrontEdge(tempEdge);
					tempNeighbour = boardTiles[indicator.wallFront[i] + 1];
					if (ensure(tempNeighbour)) {
						tempNeighbour->BindBackEdge(tempEdge);
					}
					// record the edge for saving purpose
					specialEdges.Add(tempEdge);
				}
			}
		}
	}

	// Now setup doors on indicated location's right side
	tempTile = nullptr;
	tempNeighbour = nullptr;
	tempEdge = nullptr;
	for (int32 i = 0; i < indicator.doorRight.Num(); i++) {
		if (indicator.doorRight[i] > -1) {
			tempTile = boardTiles[indicator.doorRight[i]];
			if (ensure(tempTile)) {
				// build the wall
				tempEdge = tempTile->BuildEdgeRight(2);
				if (ensure(tempEdge)) {
					tempEdge->BindBoard(this);
					// bind the wall
					tempTile->BindRightEdge(tempEdge);
					tempNeighbour = boardTiles[indicator.doorRight[i] + boardLength];
					if (ensure(tempNeighbour)) {
						tempNeighbour->BindLeftEdge(tempEdge);
					}
					// record the edge for saving purpose
					specialEdges.Add(tempEdge);
				}
			}
		}
	}

	// Now setup doors on indicated location's front side
	tempTile = nullptr;
	tempNeighbour = nullptr;
	tempEdge = nullptr;
	for (int32 i = 0; i < indicator.doorFront.Num(); i++) {
		if (indicator.doorFront[i] > -1) {
			tempTile = boardTiles[indicator.doorFront[i]];
			if (ensure(tempTile)) {
				// build the wall
				tempEdge = tempTile->BuildEdgeFront(2);
				if (ensure(tempEdge)) {
					tempEdge->BindBoard(this);
					// bind the wall
					tempTile->BindFrontEdge(tempEdge);
					tempNeighbour = boardTiles[indicator.doorFront[i] + 1];
					if (ensure(tempNeighbour)) {
						tempNeighbour->BindBackEdge(tempEdge);
					}
					// record the edge for saving purpose
					specialEdges.Add(tempEdge);
				}
			}
		}
	}

	// Now delete all invalide items from the surrounding edegs array
	for (AEdgeUnit* tempEdge : surroundingEdges) {
		if (!tempEdge) {
			surroundingEdges.Remove(tempEdge);
		}
		else {
			if (tempEdge->IsPendingKillOrUnreachable()) {
				surroundingEdges.Remove(tempEdge);
			}
		}
	}

}

void AGameBoard::GenerateRandom()
{
	// get and generate a random map from instance data set
	if (HasAuthority()) {
		// as the random generation of the game is called, also randomly generate a maximum health of the board
		MAX_HEALTH = FMath::RandRange(24, 50);
		health = MAX_HEALTH;
		UFlashPointGameInstance* gameInst = Cast<UFlashPointGameInstance>(GetGameInstance());
		if (ensure(gameInst)) {
			GenerateSpecified(gameInst->GetRandomMap());
		}
	}
}

void AGameBoard::GenerateSaved()
{
	if (!HasAuthority()) return;
	// Check which saved game is in game instance
	UFlashPointGameInstance* gameInst = Cast<UFlashPointGameInstance>(GetGameInstance());
	if (ensure(gameInst)) {
		// load the game from loaded game
		FMapSaveInfo loadedMap = gameInst->GetLoadedGame();
		if (loadedMap.isValidSave) {
			GenerateSpecified(loadedMap.boardInfo.indicator);
			// Assign to attributes the save values
			MAX_HEALTH = loadedMap.boardInfo.maxHealth;
			victimLostNum = loadedMap.boardInfo.victimLostNum;
			maxSavedVictim = loadedMap.boardInfo.maxSavedVictim;
			victimSavedNum = loadedMap.boardInfo.victimSavedNum;
			maxLostVictim = loadedMap.boardInfo.maxLostVictim;
			maxPOI = loadedMap.boardInfo.maxPOI;
			falseAlarmNum = loadedMap.boardInfo.falseAlarmNum;
			totalVictimNum = loadedMap.boardInfo.totalVictimNum;
			removedHazmat = loadedMap.boardInfo.removedHazmat;
			storedIndicator = loadedMap.boardInfo.indicator;
			gameModeType = loadedMap.boardInfo.gameModeType;
			gameMap = loadedMap.map;

			// Assign to all edges their corresponding attributes
			for (int32 i = 0; i < specialEdges.Num(); i++) {
				if (i >= loadedMap.edgesInfo.Num()) {
					break;
				}
				AEdgeUnit* tempEdge = specialEdges[i];
				if (ensure(tempEdge)) {
					tempEdge->LoadEdge(loadedMap.edgesInfo[i]);
				}
			}

			// Assign to all borders their corresponding attributes
			for (int32 i = 0; i < surroundingEdges.Num(); i++) {
				if (i >= loadedMap.bordersInfo.Num()) {
					break;
				}
				AEdgeUnit* tempEdge = surroundingEdges[i];
				if (ensure(tempEdge)) {
					tempEdge->LoadEdge(loadedMap.bordersInfo[i]);
				}
			}

			// Assign to all tiles their corresponding attributes
			for (int32 i = 0; i < boardTiles.Num(); i++) {
				if (i >= loadedMap.tilesInfo.Num()) {
					break;
				}
				ATile* tempTile = boardTiles[i];
				if (ensure(tempTile)) {
					tempTile->LoadTile(loadedMap.tilesInfo[i]);
				}
			}

			// When damages done to walls are done, adjust board health
			health = loadedMap.boardInfo.currentHealth;
		}
	}
}

void AGameBoard::RefreshBoard_Implementation()
{
	ClearAllTile();
}

void AGameBoard::Rep_TurnNotify()
{
	if (!ensure(localPlayer)) return;
	if (currentTurn == localPlayer->GetTurnNum()) {
		localPlayer->NotifyPlayerTurn();
	}
}

void AGameBoard::Rep_RescuedNotify()
{
	if (victimSavedNum >= maxSavedVictim) {
		if (ensure(localPlayer)) {
			localPlayer->NotifyGameOver(true);
		}
	}
}

void AGameBoard::Rep_HealthChangeNotify()
{
	if (health <= 0) {
		if (ensure(localPlayer)) {
			localPlayer->NotifyGameOver(false);
		}
	}
}

void AGameBoard::Rep_RelocateAmbulance()
{
	if (ensure(ambulanceLocA) && ensure(ambulance))
	{
		ambulanceLocA->setAmbulanceLocation(ambulance);
	}
}

void AGameBoard::Rep_RelocateEngine()
{
	if (ensure(engineLocA) && ensure(fireEngine))
	{
		engineLocA->setFireEngineLocation(fireEngine);
	}
}

void AGameBoard::Rep_SomeOneLeft()
{
	if (!ensure(localPlayer)) return;
	if (leaveFlag >= 0 && leaveFlag == localPlayer->GetTurnNum()) {
		localPlayer->ClientTravel("/Game/maps/MainMenu", ETravelType::TRAVEL_Absolute);
		localPlayer->LeaveGameSession();
	}
	else {
		// notify the client that someone has left the game
		localPlayer->NotifyGameForcedStop();
	}
}

void AGameBoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// mark all replicated valus
	DOREPLIFETIME(AGameBoard, boardTiles);
	DOREPLIFETIME(AGameBoard, ambulanceTiles);
	DOREPLIFETIME(AGameBoard, engineTiles);
	DOREPLIFETIME(AGameBoard, fireFighterPawns);
	DOREPLIFETIME(AGameBoard, players);
	DOREPLIFETIME(AGameBoard, health);
	DOREPLIFETIME(AGameBoard, joinedPlayerNum);
	DOREPLIFETIME(AGameBoard, placedNum);
	DOREPLIFETIME(AGameBoard, currentTurn);
	DOREPLIFETIME(AGameBoard, maxPOI);
	DOREPLIFETIME(AGameBoard, currentPOI);
	DOREPLIFETIME(AGameBoard, totalVictimNum);
	DOREPLIFETIME(AGameBoard, victimSavedNum);
	DOREPLIFETIME(AGameBoard, victimLostNum);
	DOREPLIFETIME(AGameBoard, falseAlarmNum);
	DOREPLIFETIME(AGameBoard, storedIndicator);
	DOREPLIFETIME(AGameBoard, removedHazmat);
	DOREPLIFETIME(AGameBoard, gameModeType);
	DOREPLIFETIME(AGameBoard, specialEdges);
	DOREPLIFETIME(AGameBoard, surroundingEdges);
	DOREPLIFETIME(AGameBoard, engineLocA);
	DOREPLIFETIME(AGameBoard, engineLocB);
	DOREPLIFETIME(AGameBoard, ambulanceLocA);
	DOREPLIFETIME(AGameBoard, ambulanceLocB);
	DOREPLIFETIME(AGameBoard, veteranLoc);
	DOREPLIFETIME(AGameBoard, ambulance);
	DOREPLIFETIME(AGameBoard, fireEngine);
	DOREPLIFETIME(AGameBoard, gameMap);
	DOREPLIFETIME(AGameBoard, leaveFlag);
	DOREPLIFETIME(AGameBoard, MAX_HEALTH);
}

void AGameBoard::InitializeBoardAttributes()
{
	if (HasAuthority()) {
		int32 randomPosition = 0;

		// load the initialized value from the 
		UFlashPointGameInstance* gameInst = Cast<UFlashPointGameInstance>(GetGameInstance());
		if (ensure(gameInst)) {
			ExplosionInitializeNum = gameInst->GetLobbyInfo().initialExplosions;
			HazmatInitializeNum = gameInst->GetLobbyInfo().initialHazmatNum;
		}

		if (isRandom)
		{
			// Initialize the fire
			randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
			for (size_t i = 0; i < FireInitializeNum; i++) // Initialize 3 fires
			{
				while (boardTiles[randomPosition]->IsOutside() || boardTiles[randomPosition]->GetFireStatus() == EFireStatus::Fire)
				{
					randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
				}
				boardTiles[randomPosition]->AdvanceFire();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Initialize fire un randomly."));
			// Initialize the fire
			for (FLocation loc : fireInitializeLocation)
			{
				boardTiles[loc.xLoc * boardLength + loc.yLoc]->AdvanceFire();
			}
		}

		if (gameModeType == EGameType::Experienced)
		{
			// Initialize the explosion
			// fist explosion
			randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
			while (boardTiles[randomPosition]->GetFireStatus() != EFireStatus::Fire)
			{
				randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
			}
			UE_LOG(LogTemp, Warning, TEXT("First explosion: Advance hot spot on %s"), *boardTiles[randomPosition]->GetName());
			boardTiles[randomPosition]->SetFireStatus(EFireStatus::Fire);
			boardTiles[randomPosition]->GetFireEffect()->Activate();
			boardTiles[randomPosition]->AdvanceExplosion();

			// next explosions
			for (size_t i = 0; i < ExplosionInitializeNum - 1; i++)
			{
				randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
				while (boardTiles[randomPosition]->IsOutside() || boardTiles[randomPosition]->GetFireStatus() == EFireStatus::Fire)
				{
					randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
				}
				UE_LOG(LogTemp, Warning, TEXT("Advance hot spot on %s"), *boardTiles[randomPosition]->GetName());
				boardTiles[randomPosition]->SetFireStatus(EFireStatus::Fire);
				boardTiles[randomPosition]->GetFireEffect()->Activate();
				boardTiles[randomPosition]->AdvanceExplosion();
			}
		}
		
		// TODO fix the type
		if (!isRandom && gameModeType == EGameType::Family)
		{
			// Initialize the POI
			for (FLocation loc : POIInitializeLocation)
			{
				boardTiles[loc.xLoc * boardLength + loc.yLoc]->AdvancePOI();
			}

			// Initialize the Hazmat
			for (FLocation loc : HazmatInitializeLocation)
			{
				// boardTiles[loc.xLoc * boardLength + loc.yLoc]->AdvanceHazmat();
			}			
		}
		else
		{
			// Initialize the POI
			for (size_t i = 0; i < POIInitializeNum; i++)
			{
				while (boardTiles[randomPosition]->IsOutside() ||
					boardTiles[randomPosition]->GetFireStatus() == EFireStatus::Fire ||
					boardTiles[randomPosition]->GetPOIStatus() != EPOIStatus::Empty)
				{
					randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
				}
				boardTiles[randomPosition]->AdvancePOI();
			}
		}


		// Initialize the Hazmat
		// TODO check the game mode
		if (gameModeType != EGameType::Family) {
			for (size_t i = 0; i < HazmatInitializeNum; i++)
			{
				randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
				while (boardTiles[randomPosition]->IsOutside() ||
					boardTiles[randomPosition]->GetFireStatus() == EFireStatus::Fire ||
					boardTiles[randomPosition]->GetPOIStatus() != EPOIStatus::Empty ||
					boardTiles[randomPosition]->GetHazmat() != nullptr)
				{
					randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
				}
				boardTiles[randomPosition]->AdvanceHazmat();
			}
		}
		currentPOI = maxPOI;
	}
}

void AGameBoard::RelocateCamera()
{
	// relocate each player's camera
	if (ensure(CameraClass)) {
		AViewPortCamera* camera = GetWorld()->SpawnActor<AViewPortCamera>(
			CameraClass,
			FVector(boardWidth * TILE_SIZE / 2 - TILE_SIZE, boardLength * TILE_SIZE / 2 + TILE_SIZE, camHeight),
			FRotator(0, 0, 0)
			);
		camera->RelocateAndSetToViewPort(FVector(boardWidth * TILE_SIZE / 2 - TILE_SIZE, boardLength * TILE_SIZE / 2 + TILE_SIZE, camHeight));
	}
}

// Called when the game starts or when spawned
void AGameBoard::BeginPlay()
{
	// Set replicate for server and client synchronization
	if (HasAuthority()) {
		SetReplicates(true);

		// Initialize starting game map
		InitializeDefaultBoard();

		// Initialize game board health
		health = MAX_HEALTH;
	}
	// Initialize all players in the game
	AFPPlayerController* tempPlayer = nullptr;
	for (FConstPlayerControllerIterator iterator = GetWorld()->GetPlayerControllerIterator(); iterator; ++iterator) {
		// Get each player controller
		tempPlayer = Cast<AFPPlayerController>(iterator->Get());
		if (ensure(tempPlayer)) {
			tempPlayer->SetInputMode(FInputModeGameAndUI());
			players.Add(tempPlayer);
			// Set the player's action mode to place initial fire fighter
			tempPlayer->SetPlaceFireFighter();
			UE_LOG(LogTemp, Warning, TEXT("relocating camera on player %s"), *tempPlayer->GetName());
			// Detach and relocate the player controller's camera
			//Cast<AFireFighterPawn>(tempPlayer->GetPawn())->RelocateCamera(GetActorLocation() + FVector(boardWidth * TILE_SIZE / 2 - TILE_SIZE, boardLength * TILE_SIZE / 2 + TILE_SIZE, camHeight));
		}
	}
	Super::BeginPlay();

	//RefreshBoard();

	// get the local player reference
	localPlayer = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
}

void AGameBoard::setPOIalarm(APOI* inPOI)
{
	bool placedSuccess = false;
	while (!placedSuccess)
	{
		if (FMath::RandRange(-2, 2) > 0)
		{
			if (falseAlarmNum)
			{
				UE_LOG(LogTemp, Warning, TEXT("isfalseAlarm"));
				inPOI->isAlarm = false;
				falseAlarmNum--;
				UE_LOG(LogTemp, Warning, TEXT("%d"), falseAlarmNum);
				placedSuccess = true;
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("isAlarm"));
			if (totalVictimNum)
			{
				inPOI->isAlarm = true;
				totalVictimNum--;
				UE_LOG(LogTemp, Warning, TEXT("%d"), totalVictimNum);
				placedSuccess = true;
			}
		}
	}
}

FMapSaveInfo AGameBoard::SaveCurrentMap()
{
	FMapSaveInfo savedMap;
	// Save the board for board regeneration
	savedMap.boardInfo = SaveCurrentBoard();
	// save the current map's name
	savedMap.map = gameMap;
	// Save all the edge informations
	for (AEdgeUnit* tempEdge : specialEdges) {
		if (ensure(tempEdge)) {
			savedMap.edgesInfo.Add(tempEdge->SaveEdge());
		}
	}
	// Save all border edges
	for (AEdgeUnit* tempEdge : surroundingEdges) {
		if (ensure(tempEdge)) {
			savedMap.bordersInfo.Add(tempEdge->SaveEdge());
		}
	}
	// Save all tiles with their informations
	for (ATile* tempTile : boardTiles) {
		if (ensure(tempTile)) {
			savedMap.tilesInfo.Add(tempTile->SaveTile());
		}
	}
	// Save the date of the saving
	FString currentDate = FDateTime::Now().GetDate().ToString();
	UE_LOG(LogTemp, Warning, TEXT("Save date is: %s"), *currentDate);
	savedMap.saveDate = currentDate;
	savedMap.isValidSave = true;
	return savedMap;
}

FBoardSaveInfo AGameBoard::SaveCurrentBoard()
{
	// create such a struct for saving
	FBoardSaveInfo savedBoard;

	// Assign to the board save info the corresponding values
	savedBoard.currentHealth = health;
	savedBoard.maxHealth = MAX_HEALTH;
	savedBoard.victimLostNum = victimLostNum;
	savedBoard.maxLostVictim = maxLostVictim;
	savedBoard.victimSavedNum = victimSavedNum;
	savedBoard.maxSavedVictim = maxSavedVictim;
	savedBoard.maxPOI = maxPOI;
	// Save the false alarm num and total victim num including those are placed and not revealed
	savedBoard.falseAlarmNum = falseAlarmNum;
	savedBoard.totalVictimNum = totalVictimNum;
	for (ATile* tempTile : boardTiles) {
		if (tempTile->GetPOIStatus() == EPOIStatus::Hided) {
			APOI* tempPOI = tempTile->GetPOIOnTile();
			if (ensure(tempPOI)) {
				if (tempPOI->isAlarm) {
					savedBoard.totalVictimNum = savedBoard.totalVictimNum + 1;
				}
				else {
					savedBoard.falseAlarmNum = savedBoard.falseAlarmNum + 1;
				}
			}
		}
	}
	savedBoard.removedHazmat = removedHazmat;
	savedBoard.indicator = storedIndicator;
	savedBoard.gameModeType = gameModeType;

	return savedBoard;
}

ATile * AGameBoard::GenerateRandomPositionInQuadrant()
{
	int32 startx = -1;
	int32 starty = -1;
	if (ensure(engineLocA))
	{
		engineLocA->GetLocation(startx, starty);
		UE_LOG(LogTemp, Warning, TEXT("Start Position $d, %d"), startx, starty);
	}
	if (startx <= 4) { startx = 1; }
	else { startx = 5; }
	if (starty <= 3) { starty = 1; }
	else { starty = 4; }
	
	// randomly choose a target fire 
	int32 randx = FMath::RandRange(startx, startx + 3);
	int32 randy = FMath::RandRange(starty, starty + 2);
	UE_LOG(LogTemp, Warning, TEXT("Random Position $d, %d"), randx, randy);
	return boardTiles[randx * 8 + randy];
}

// Called every frame
void AGameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameBoard::SetAmbulanceLocA(ATile* a)
{
	if (ensure(a->GetType() == ETileType::AmbulancePark))
	{
		ambulanceLocA = a;
	}
}

void AGameBoard::SetAmbulanceLocB(ATile* b)
{
	if (ensure(b->GetType() == ETileType::AmbulancePark))
	{
		ambulanceLocB = b;
	}
}

void AGameBoard::SetFireEngineLocA(ATile* a)
{
	if (ensure(a->GetType() == ETileType::FireEnginePark))
	{
		engineLocA = a;
	}
}

void AGameBoard::SetFireEngineLocB(ATile* b)
{
	if (ensure(b->GetType() == ETileType::FireEnginePark))
	{
		engineLocB = b;
	}
}

