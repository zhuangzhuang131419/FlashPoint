// Fill out your copyright notice in the Description page of Project Settings.

#include "GameBoard.h"


// Sets default values
AGameBoard::AGameBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameBoard::BeginPlay()
{
	Super::BeginPlay();
	
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

// Called every frame
void AGameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

