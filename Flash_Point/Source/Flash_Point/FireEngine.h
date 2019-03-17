// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralTypes.h"
#include "Tile.h"
#include "GameBoard.h"
#include "GameFramework/Actor.h"
#include "FireEngine.generated.h"

UCLASS()
class FLASH_POINT_API AFireEngine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireEngine();

	void FireDeckGun();

	// The parking location of the fire engine
	FLocation parkingSpot1;
	FLocation parkingSpot2;

	AGameBoard* board;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void splashOver(ATile* targetTile, EDirection direction);
};
