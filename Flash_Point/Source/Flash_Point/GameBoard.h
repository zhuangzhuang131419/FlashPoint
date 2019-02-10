// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Tile.h"
#include "GameBoard.generated.h"

UCLASS()
class FLASH_POINT_API AGameBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameBoard();

protected:
	// FIELDS
	// The tile class for spawning
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ATile> TileClass = nullptr;
	// The edge class for spawning
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AEdgeUnit> EdgeClass = nullptr;
	TArray<ATile*> boardTiles;

	// FUNCTIONS
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// PRIVATE FIELDS
	const int32 boardWidth = 10;
	const int32 boardLength = 8;
	const int32 TILE_SIZE = 400;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
