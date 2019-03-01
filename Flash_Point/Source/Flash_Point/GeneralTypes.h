// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralTypes.generated.h"

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

class FLASH_POINT_API GeneralTypes
{
public:
	GeneralTypes();
	~GeneralTypes();
};
