// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Tile.h"
#include "ViewPortCamera.h"
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
	int32 GetCurrentGameHealth() { return health; }
	UFUNCTION(BlueprintCallable, Category = "Map Attributes")
	void SetCurrentGameHealth(int32 currentHealth);
	// Get current turn number
	UFUNCTION(BlueprintCallable, Category = "Map Attributes")
	int32 GetCurrentTurn();
	// Function for checking if an tile is where engine is placed at
	UFUNCTION(BlueprintCallable, Category = "Map Attributes")
	bool IsEngineTile(ATile* inTile);
	// Function for checking if an tile is where ambulance is placed at
	UFUNCTION(BlueprintCallable, Category = "Map Attributes")
	bool IsAmbulanceTile(ATile* inTile);
	// Function for checking if an tile is where ambulance is placed at
	UFUNCTION(BlueprintCallable, Category = "Map Attributes")
	EGameType GetGameType() { return gameModeType; }

	ATile* GetAmbulanceLocA() { return ambulanceLocA; }
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void SetAmbulanceLocA(ATile* a);
	ATile* GetAmbulanceLocB() { return ambulanceLocB; }
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void SetAmbulanceLocB(ATile* b);
	ATile* GetFireEngineLocA() { return engineLocA; }
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void SetFireEngineLocA(ATile* a);
	ATile* GetFireEngineLocB() { return engineLocB; }
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void SetFireEngineLocB(ATile* b);

	int32 GetBoardWidth() { return boardWidth; }
	int32 GetBoardLength() { return boardLength; }

	// Getter for ambulanceTile
	TArray<ATile*> GetAmbulanceTiles() { return ambulanceTiles; }

	// Getter for engineTile
	TArray<ATile*> GetEngineTiles() { return engineTiles; }

	// Getter and setter for veteran location
	ATile* GetVeteranLoc() { return veteranLoc; }
	void SetVeteranLoc(ATile* current) { veteranLoc = current; }

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
	void flashover();

	void ResolveKnockDownOnBoard();
	void resolveKnockDown();

	void ResolveHazmatExplosionOnBoard();
	void resolveHazmatExplosions();

	UFUNCTION(BlueprintCallable)
	void endTurnRelatedOperations();

	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	ATile* ambulanceLocA = nullptr;
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	ATile* ambulanceLocB = nullptr;
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	ATile* engineLocA = nullptr;
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	ATile* engineLocB = nullptr;

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

	UPROPERTY(BlueprintReadWrite, Category = "Vehicle")
	bool moved = true;

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

	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadWrite, Category = "Setup")
	FSpawnIndicator storedIndicator;

	UPROPERTY(replicated, EditAnyWhere, BlueprintReadWrite, Category = "Setup")
	int32 removedHazmat = 0;

	TArray<ATile*> GetboardTiles() { return boardTiles; }

	// Getter and setter for initial explosion
	int32 GetExplosionInitializeNum() { return ExplosionInitializeNum; }
	void SetExplosionInitializeNum(int32 current) { ExplosionInitializeNum = current; }
	int32 GetHazmatInitializeNum() { return HazmatInitializeNum; }
	void SetHazmatInitializeNum(int32 current) { HazmatInitializeNum = current; }

	// Getter and setter for ambulance and fire engine
	AAmbulance* GetAmbulance() { return ambulance; }
	void SetAmbulance(AAmbulance* current) { ambulance = current; }

	AFireEngine* GetFireEngine() { return fireEngine; }
	void SetFireEngine(AFireEngine* current) { fireEngine = current; }


	// Save related functions
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	FMapSaveInfo SaveCurrentMap();
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	FBoardSaveInfo SaveCurrentBoard();


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
	// A Hazmat actor to spawn on tiles
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AActor> HazmatClass = nullptr;
	// A local player to call server functions on
	AFPPlayerController* localPlayer = nullptr;
	
	// The road tile class for spawning
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	AFPPlayerController* currentPlayer = nullptr;
	// A resonable height for camera to be above the board
	UPROPERTY(EditAnyWhere, Category = "Setup")
	int32 camHeight = 3500;

	// whether initialize fire and POI randomly or not
	UPROPERTY(EditAnyWhere, Category = "Setup")
	bool isRandom = false;

	UPROPERTY(replicated, EditAnyWhere, Category = "Setup")
	EGameType gameModeType = EGameType::Experienced;

	UPROPERTY(EditAnyWhere, Category = "Setup")
	TArray<FLocation> fireInitializeLocation;

	UPROPERTY(EditAnyWhere, Category = "Setup")
	TArray<FLocation> POIInitializeLocation;

	UPROPERTY(EditAnyWhere, Category = "Setup")
	TArray<FLocation> HazmatInitializeLocation;

	// the entire board are stored here
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<ATile*> boardTiles;
	// the later added edges are stored here
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<AEdgeUnit*> specialEdges;
	// the surrounding edges are stored here
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<AEdgeUnit*> surroundingEdges;
	// the ambulance parking tiles are stored here
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<ATile*> ambulanceTiles;
	// the engine parking tiles are stored here
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<ATile*> engineTiles;
	// The firefighters placed on the board
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<AFireFighterPawn*> fireFighterPawns;
	// All players in current map
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	TArray<AFPPlayerController*> players;
	// The current gameboard health
	UPROPERTY(ReplicatedUsing = Rep_HealthChangeNotify, EditAnyWhere, Category = "Map Attributes")
	int32 health = MAX_HEALTH;
	// locations of the fire engine
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	ATile* veteranLoc = nullptr;

	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	AAmbulance* ambulance = nullptr;
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Attributes")
	AFireEngine* fireEngine = nullptr;
	
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
	// For blueprint to call on in order to generate a map that's random
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void GenerateRandom();
	// For blueprint to call on in order to generate a map that's random
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void GenerateSaved();
	// For blueprint to call on in order to initialize properties on board
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void InitializeBoardAttributes();
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
	int32 HazmatInitializeNum = 3;
	const int32 HotSpotInitializeNum = 3;
	int32 ExplosionInitializeNum = 3;

	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
