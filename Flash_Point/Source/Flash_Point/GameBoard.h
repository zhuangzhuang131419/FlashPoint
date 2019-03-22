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
	// getter and setter for selected roles on board
	UFUNCTION(BlueprintCallable, Category = "Map Attributes")
	TArray<ERoleType> GetSelectedRoles();
	UFUNCTION(BlueprintCallable, Category = "Map Attributes")
	void AddToSelectedRoles(ERoleType inRole);
	UFUNCTION(BlueprintCallable, Category = "Map Attributes")
	void SwitchRolesFromTo(ERoleType fromRole, ERoleType toRole);
	// Get current turn number
	UFUNCTION(BlueprintCallable, Category = "Map Attributes")
	int32 GetCurrentTurn();

	int32 GetBoardWidth() { return boardWidth; }
	int32 GetBoardLength() { return boardLength; }

	// Getter for ambulanceTile
	TArray<ATile*> GetAmbulanceTiles() { return ambulanceTiles; }

	// Getter for engineTile
	TArray<ATile*> GetEngineTiles() { return engineTiles; }

	// to clear all tile status, used in path finding
	void ClearAllTile();
	// below functions are used to advance fire and replenish POI
	void AdvanceFireOnBoard();
	UFUNCTION(BlueprintCallable)
	void AdvanceFire();
	void AdvancePOIOnBoard();
	UFUNCTION(BlueprintCallable)
	void AdvancePOI();
	// for server to call in order to do flash over
	void FlashOverOnBoard();
	UFUNCTION(BlueprintCallable)
	void flashover();

	// Functions of turn related operaetions
	UFUNCTION(BlueprintCallable, Category = "Take turn")
	int32 JoinBoard();
	UFUNCTION(BlueprintCallable, Category = "Take turn")
	void TurnSwitch();
	UFUNCTION(BlueprintCallable, Category = "Take turn")
	void InitialPlacing();

	// Set alarm type for POI
	void setPOIalarm(APOI* inPOI);

	// Max health of the board to be specified at begining
	UPROPERTY(BlueprintReadWrite, Category = "Map Attributes")
	int32 MAX_HEALTH = 24;

	// Operation realted fields and functions
	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadOnly, Category = "Setup")
	int32 victimLostNum = 0;

	void SetVictimLostNum(int32 num) { victimLostNum = num; }

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	int32 maxLostVictim = 4;

	UPROPERTY(ReplicatedUsing = Rep_RescuedNotify, VisibleAnyWhere, BlueprintReadOnly, Category = "Setup")
	int32 victimSavedNum = 0;

	void SetVictimSavedNum(int32 num) { victimSavedNum = num; }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Setup")
	int32 maxSavedVictim = 7;

	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadOnly, Category = "Setup")
	int32 currentPOI = 0;

	void SetCurrentPOI(int32 num) { currentPOI = num; }
		 
	UPROPERTY(replicated, EditAnyWhere, BlueprintReadWrite, Category = "Setup")
	int32 maxPOI = 3;

	UPROPERTY(replicated, EditAnyWhere, BlueprintReadWrite, Category = "Setup")
	int32 falseAlarmNum = 5;

	UPROPERTY(replicated, EditAnyWhere, BlueprintReadWrite, Category = "Setup")
	int32 totalVictimNum = 10;

	TArray<ATile*> GetboardTiles() { return boardTiles; }

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
	// A local player to call server functions on
	AFPPlayerController* localPlayer = nullptr;
	
	// The road tile class for spawning
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	AFPPlayerController* currentPlayer = nullptr;
	// A resonable height for camera to be above the board
	UPROPERTY(EditAnyWhere, Category = "Setup")
	int32 camHeight = 2900;

	// whether initialize fire and POI randomly or not
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	bool isRandom = false;

	UPROPERTY(EditAnyWhere, Category = "Setup")
	TArray<FLocation> fireInitializeLocation;

	UPROPERTY(EditAnyWhere, Category = "Setup")
	TArray<FLocation> POIInitializeLocation;

	// the entire board are stored here
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<ATile*> boardTiles;
	// the ambulance parking tiles are stored here
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<ATile*> ambulanceTiles;
	// the engine parking tiles are stored here
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<ATile*> engineTiles;
	// The firefighters placed on the board
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<AFireFighterPawn*> fireFighters;
	// All players in current map
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<AFPPlayerController*> players;
	// Already selected roles in the game
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<ERoleType> selectedRoles;
	// The current gameboard health
	UPROPERTY(ReplicatedUsing = Rep_HealthChangeNotify, EditAnyWhere, Category = "Map Attributes")
	int32 health = MAX_HEALTH;
	// TURN RELATED FIELDS
	// A to indicate how many player are currently in the game
	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadWrite, Category = "Map Attributes")
	int32 joinedPlayerNum = 0;
	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadWrite, Category = "Map Attributes")
	int32 placedNum = 0;	// when this number is the same as joined number, the game can start by changing turn
	UPROPERTY(ReplicatedUsing = Rep_TurnNotify, VisibleAnyWhere, BlueprintReadWrite, Category = "Map Attributes")
	int32 currentTurn = -1;	// this is initialized to -1 to indicate a placing firefighter state

	// FUNCTIONS
	// This method will initilize the default board
	void InitializeDefaultBoard();
	// For blueprint to call on in order to generate a map as specified with indicator
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void GenerateSpecified(FSpawnIndicator indicator);
	// A function to refresh synchronized board
	UFUNCTION(Client, Reliable)
	void RefreshBoard();

	// REPLICATION FUNCTIONS
	UFUNCTION()
	void Rep_TurnNotify();
	UFUNCTION()
	void Rep_RescuedNotify();
	UFUNCTION()
	void Rep_HealthChangeNotify();

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
