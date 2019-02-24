// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralTypes.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGameOperations : uint8
{
	PlaceFireFighter,
	Move,
	ChopWall,
	ExtinguishFire,
	Carry,
	OpenDoor,
	None
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
	// An int array to indicate to which tile's left has a wall on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> wallLeft;
	// An int array to indicate to which tile's front has a wall on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> wallFront;
	// An int array to indicate to which door's left has a wall on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> doorLeft;
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
