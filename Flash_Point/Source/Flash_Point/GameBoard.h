// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Tile.h"
#include "ViewPortCamera.h"
#include "FPPlayerController.h"
#include "FireFighterPawn.h"
#include "GameBoard.generated.h"

UCLASS()
class FLASH_POINT_API AGameBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameBoard();
	// getter and setter for current player controller in order to take turn
	UFUNCTION(BlueprintCallable, Category = "Take turn")
	AFPPlayerController* GetCurrentPlayer();
	UFUNCTION(BlueprintCallable, Category = "Take turn")
	void SetCurrentPlayer(AFPPlayerController* current);
	// getter and settter for current game board health
	UFUNCTION(BlueprintCallable, Category = "Map Attributes")
	int32 GetCurrentGameHealth();
	UFUNCTION(BlueprintCallable, Category = "Map Attributes")
	void SetCurrentGameHealth(int32 currentHealth);
	// to clear all tile status, used in path finding
	void ClearAllTile();

	UFUNCTION(BlueprintCallable)
	void AdvanceFire();

	UFUNCTION(BlueprintCallable)
	void AdvancePOI();

	// Max health of the board to be specified at begining
	UPROPERTY(BlueprintReadOnly, Category = "Map Attributes")
	int32 MAX_HEALTH = 24;

	UPROPERTY(BlueprintReadOnly)
	int32 victimNum = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 maxVictim = 4;

protected:
	// FIELDS
	// The tile class for spawning
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ATile> TileClass = nullptr;
	// The road tile class for spawning
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ATile> RoadClass = nullptr;
	// A camera class to relocate player view port to
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AViewPortCamera> CameraClass = nullptr;
	// A POI actor to spawn on tiles
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AActor> POIClass = nullptr;
	
	// The road tile class for spawning
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	AFPPlayerController* currentPlayer = nullptr;
	// A resonable height for camera to be above the board
	UPROPERTY(EditAnyWhere, Category = "Setup")
	int32 camHeight = 2800;
	// the entire board are stored here
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<ATile*> boardTiles;
	// The firefighters placed on the board
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<AFireFighterPawn*> fireFighters;
	// All players in current map
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<AFPPlayerController*> players;
	// The current gameboard health
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	int32 health = MAX_HEALTH;

	// FUNCTIONS
	// This method will initilize the default board
	void InitializeDefaultBoard();
	// For blueprint to call on in order to generate a map as specified with indicator
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void GenerateSpecified(FSpawnIndicator indicator);
	// A function to refresh synchronized board
	UFUNCTION(Client, Reliable)
	void RefreshBoard();

	void AdvanceExplosion();

	// Called when the game starts or when spawned
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual void BeginPlay() override;

private:
	void InitializeFire();

	// PRIVATE FIELDS
	const int32 boardWidth = 10;
	const int32 boardLength = 8;
	const int32 TILE_SIZE = 400;
	const int32 FireInitializeNum = 10;
	const int32 POIInitializeNum = 3;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
