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
	if (ensure(TileClass)) {
		for (int32 i = 0; i < boardWidth; i++) {
			for (int32 j = 0; j < boardLength; j++) {
				FVector tempLoc = FVector(TILE_SIZE * j, TILE_SIZE * i, 0) + GetActorLocation();
				boardTiles.Add(GetWorld()->SpawnActor<ATile>(
					TileClass,
					tempLoc,
					GetActorRotation()
					));
			}
		}
	}
}

// Called every frame
void AGameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

