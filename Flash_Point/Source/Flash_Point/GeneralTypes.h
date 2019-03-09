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
enum class EGameOperations : uint8
{
	PlaceFireFighter,
	RespawnFireFighter,
	Move,
	ChopWall,
	ExtinguishFire,
	Carry,
	OpenDoor,
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
