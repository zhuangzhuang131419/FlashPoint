// Fill out your copyright notice in the Description page of Project Settings.

#include "GameBoard.h"


// Sets default values
AGameBoard::AGameBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AFPPlayerController * AGameBoard::GetCurrentPlayer()
{
	return currentPlayer;
}

void AGameBoard::SetCurrentPlayer(AFPPlayerController * current)
{
	currentPlayer = current;
}

int32 AGameBoard::GetCurrentGameHealth()
{
	return health;
}

void AGameBoard::SetCurrentGameHealth(int32 currentHealth)
{
	health = currentHealth;
}

void AGameBoard::ClearAllTile()
{
	for (ATile* t : boardTiles) {
		if (ensure(t)) {
			t->ResetTile();
		}
	}
}

void AGameBoard::AdvanceFire()
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
		boardTiles[randomPosition]->GetBlastEffect()->Activate();
		boardTiles[randomPosition]->GetBlastEffect()->Deactivate();
	}
}

void AGameBoard::AdvancePOI()
{
	int32 randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
	while (boardTiles[randomPosition]->GetPOIStatus() != EPOIStatus::Empty || boardTiles[randomPosition]->IsOutside())
	{
		randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
	}
	if (boardTiles[randomPosition]->GetFireStatus() == EFireStatus::Fire) 
	{
		victimNum++;
	}
	else {
		boardTiles[randomPosition]->SetPOIStatus(EPOIStatus::Hided);
		FVector POISocketLocation = boardTiles[randomPosition]->GetTileMesh()->GetSocketLocation(FName("POI"));
		AActor* POI = GetWorld()->SpawnActor<AActor>(
			POIClass,
			POISocketLocation,
			FRotator(0, 0, 0)
			);
		boardTiles[randomPosition]->SetPOIOnTile(POI);
	}

	UE_LOG(LogTemp, Warning, TEXT("Advance POI."));
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
}

void AGameBoard::GenerateSpecified(FSpawnIndicator indicator)
{
	if (!HasAuthority()) return;
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
				}
			}
		}
	}

	// Now set up specific tiles for fire engine park or ambulance park
	tempTile = nullptr;
	for (int32 i = 0; i < indicator.engineParkLoc.Num(); i++) {
		if (indicator.engineParkLoc[i] > -1) {
			// set up for the first tile on the specified location
			tempTile = boardTiles[indicator.engineParkLoc[i]];
			if (ensure(tempTile)) {
				tempTile->SetTileType(ETileType::FireEnginePark);
			}
			// set up for the second tile on the specified location
			if (i % 4 == 0 || i % 4 == 1) {
				tempTile = boardTiles[indicator.engineParkLoc[i] + boardLength];
			}
			else {
				tempTile = boardTiles[indicator.engineParkLoc[i] + 1];
			}
			if (ensure(tempTile)) {
				tempTile->SetTileType(ETileType::FireEnginePark);
			}
		}
	}
	tempTile = nullptr;
	for (int32 i = 0; i < indicator.ambulanceParkLoc.Num(); i++) {
		if (indicator.ambulanceParkLoc[i] > -1) {
			tempTile = boardTiles[indicator.ambulanceParkLoc[i]];
			if (ensure(tempTile)) {
				tempTile->SetTileType(ETileType::AmbulancePark);
			}
			// set up for the second tile on the specified location
			if (i % 4 == 0 || i % 4 == 1) {
				tempTile = boardTiles[indicator.ambulanceParkLoc[i] + boardLength];
			}
			else {
				tempTile = boardTiles[indicator.ambulanceParkLoc[i] + 1];
			}
			if (ensure(tempTile)) {
				tempTile->SetTileType(ETileType::AmbulancePark);
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
					tempNeighbour = boardTiles[indicator.wallRight[i] + boardLength];
					if (ensure(tempNeighbour)) {
						tempNeighbour->BindLeftEdge(tempEdge);
					}
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
				}
			}
		}
	}
}

void AGameBoard::RefreshBoard_Implementation()
{
	ClearAllTile();
}

void AGameBoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// mark all replicated valus
	DOREPLIFETIME(AGameBoard, boardTiles);
	DOREPLIFETIME(AGameBoard, fireFighters);
	DOREPLIFETIME(AGameBoard, players);
	DOREPLIFETIME(AGameBoard, health);
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

	// Initialize the fire
	if (HasAuthority()) {
		int32 randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
		for (size_t i = 0; i < FireInitializeNum; i++) // Initialize 3 fires
		{
			while (boardTiles[randomPosition]->IsOutside() || boardTiles[randomPosition]->GetFireStatus() == EFireStatus::Fire)
			{
				randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
			}
			boardTiles[randomPosition]->AdvanceFire();
		}

		// Initialize the POI
		randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
		for (size_t i = 0; i < POIInitializeNum; i++)
		{
			while (boardTiles[randomPosition]->IsOutside() ||
				boardTiles[randomPosition]->GetFireStatus() == EFireStatus::Fire ||
				boardTiles[randomPosition]->GetPOIStatus() != EPOIStatus::Empty)
			{
				randomPosition = FMath::RandRange(0, boardTiles.Num() - 1);
			}
			boardTiles[randomPosition]->SetPOIStatus(EPOIStatus::Hided);
			FVector POISocketLocation = boardTiles[randomPosition]->GetTileMesh()->GetSocketLocation(FName("POI"));
			AActor* POI = GetWorld()->SpawnActor<AActor>(
				POIClass,
				POISocketLocation,
				FRotator(0, 0, 0)
				);
			boardTiles[randomPosition]->SetPOIOnTile(POI);
		}
	}
	


	RefreshBoard();
	// relocate each player's camera
	if (ensure(CameraClass)) {
		AViewPortCamera* camera = GetWorld()->SpawnActor<AViewPortCamera>(
			CameraClass,
			FVector(boardWidth * TILE_SIZE / 2 - TILE_SIZE, boardLength * TILE_SIZE / 2 + TILE_SIZE, camHeight),
			FRotator(270, 0, 0)
			);
		camera->RelocateAndSetToViewPort(FVector(boardWidth * TILE_SIZE / 2 - TILE_SIZE, boardLength * TILE_SIZE / 2 + TILE_SIZE, camHeight));
	}
}

// Called every frame
void AGameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

