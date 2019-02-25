// Fill out your copyright notice in the Description page of Project Settings.

#include "GameBoard.h"


// Sets default values
AGameBoard::AGameBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGameBoard::InitializeDefaultBoard()
{
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

// Called when the game starts or when spawned
void AGameBoard::BeginPlay()
{
	InitializeDefaultBoard();

	Super::BeginPlay();
}

// Called every frame
void AGameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

