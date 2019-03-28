// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralTypes.generated.h"

class ATile;

/**
 * 
 */
// An enum for different operations
UENUM(BlueprintType)
enum class ERoleType : uint8
{
	Basic,
	Paramedic,
	FireCaptain,
	ImagingTechnician,
	CAFSFirefighter,
	HazmatTechnician,
	Generalist,
	RescueSpecialist,
	Driver,
	Veteran,
	RescueDog,
};

// An enum for different operations
UENUM(BlueprintType)
enum class EGameOperations : uint8
{
	PlaceFireFighter,
	RespawnFireFighter,
	Move,
	ChopWall,
	ExtinguishFire,
	Carry,
	OpenDoor,
	FireDeckGun,
	FlipPOI,
	Dodge,
	Squeeze,
	None
};

// An enum for different tile types
UENUM(BlueprintType)
enum class ETileType : uint8
{
	Default,
	FireEnginePark,
	AmbulancePark
};

// An enum for edge types
UENUM(BlueprintType)
enum class EEdgeType : uint8
{
	Empty,
	Wall,
	Door
};

// An enum for tile status
UENUM(BlueprintType)
enum class EFireStatus : uint8
{
	Clear,
	Smoke,
	Fire
};

// An enum for POI status
UENUM(BlueprintType)
enum class EPOIStatus : uint8
{
	Empty,
	Revealed,
	Hided
};

// An enum for direction
UENUM(BlueprintType)
enum class EDirection : uint8
{
	Up,
	Down,
	Left,
	Right
};

// An enum for game mode types
UENUM(BlueprintType)
enum class EGameType : uint8
{
	Family,
	Experienced,
	Betryal
};

// A struct indicate the option prompt
UENUM(BlueprintType)
enum class EOptionPromptType : uint8
{
	Dodge,
	Command,
};

// A struct indicate the location
USTRUCT(BlueprintType)
struct FLocation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location info")
	int32 xLoc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location info")
	int32 yLoc;
	FLocation() : xLoc(0), yLoc(0) {}
	FLocation(int32 x, int32 y) 
	{
		xLoc = x;
		yLoc = y;
	}
};

// A struct used to indicate map generation
USTRUCT(BlueprintType)
struct FSpawnIndicator
{
	GENERATED_USTRUCT_BODY()

	// An integer array to specify the locations of door for the four boundaries in orde of front, back, left, right
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> doorLoc;
	// An integer array to specify the locations of fire engine parks in orde of front, back, left, right
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> engineParkLoc;
	// An integer array to specify the locations of fire engine parks in orde of front, back, left, right
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> ambulanceParkLoc;
	// An int array to indicate to which tile's right has a wall on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> wallRight;
	// An int array to indicate to which tile's front has a wall on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> wallFront;
	// An int array to indicate to which door's right has a wall on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> doorRight;
	// An int array to indicate to which door's front has a wall on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> doorFront;
};

// A struct to store nodes for a star searching on shortest path tile
USTRUCT(BlueprintType)
struct FSearchNode
{
	GENERATED_USTRUCT_BODY()
		
	// integer for expected
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search Node")
	int32 heuristic;
	// integer for actual path cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search Node")
	int32 cost;
	// tile for node tile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search Node")
	ATile* nodeTile = nullptr;

	// override the < operator to be used in heap naking
	friend bool operator<(const FSearchNode& lhs, const FSearchNode& rhs) {
		if (lhs.heuristic < rhs.heuristic) {
			return true;
		}
		else if (lhs.heuristic == rhs.heuristic && lhs.cost > rhs.cost) {
			return true;
		}
		else {
			return false;
		}
	}

	// override the == operator to be used in heap naking
	friend bool operator==(const FSearchNode& lhs, const FSearchNode& rhs) {
		if (lhs.heuristic == rhs.heuristic && lhs.cost == rhs.cost && lhs.nodeTile == rhs.nodeTile) {
			return true;
		}
		else {
			return false;
		}
	}

	FSearchNode() : heuristic(0), cost(0), nodeTile(nullptr) {}
	FSearchNode(int32 heur, int32 nodeCost, ATile* tile) : heuristic(heur), cost(nodeCost), nodeTile(tile) {}
};

// A struct to store board save info only
USTRUCT(BlueprintType)
struct FBoardSaveInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 currentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 victimLostNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 maxLostVictim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 victimSavedNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 maxSavedVictim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 maxPOI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 falseAlarmNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 totalVictimNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 removedHazmat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	FSpawnIndicator indicator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	EGameType gameModeType;

};

// A struct to store all informations on a specific edge
USTRUCT(BlueprintType)
struct FEdgeSaveInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// if the saved game is valid (default is false)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	EEdgeType type;
	// if the edge is blocked
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	bool isBlocked = true;
	// if the wall is choped or the door is opened
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	bool chopedOrOpened = false;
	// for door only, if the door is destroyed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	bool isDestroyed = false;


};

// A struct to store all informations on a specific tile
USTRUCT(BlueprintType)
struct FTileSaveInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// if the tile has POI that's not revealed on it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	bool hasPOI = false;
	// if the tile is on fire or smoke
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	EFireStatus fireStatus = EFireStatus::Clear;
	// how many victims are on that specific tile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 victimsOnTile = 0;
	// if there is a hazmat on the tile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	bool hasHazmat = false;


};

// A struct to store all informations about a saved game
USTRUCT(BlueprintType)
struct FMapSaveInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// if the saved game is valid (default is false)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	bool isValidSave = false;

	// Infomation of the board
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	FBoardSaveInfo boardInfo;
	// Information on edges within the board
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	TArray<FEdgeSaveInfo> edgesInfo;
	// Information on border edges within the board
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	TArray<FEdgeSaveInfo> bordersInfo;
	// Information on tiles within the board
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	TArray<FTileSaveInfo> tilesInfo;
	
	
};

class FLASH_POINT_API GeneralTypes
{
public:
	GeneralTypes();

	// A method to use a star search to find the shortest path from one tile to another
	static int32 AStarShotest(int32 maxDepth, ATile* start, ATile* goal, TArray<ATile*> & trace);
	// A method to find the heuristic of a node to goal
	static int32 GetHeuristic(ATile* cur, ATile* goal);

	~GeneralTypes();
};
