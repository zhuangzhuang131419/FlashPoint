// Fill out your copyright notice in the Description page of Project Settings.

#include "FireEngine.h"
#include "GameBoard.h"

// Sets default values
AFireEngine::AFireEngine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AFireEngine::FireDeckGun()
{
	UE_LOG(LogTemp, Warning, TEXT("FIRING DECK GUN!"));
	// check the quadrant has firefighter or not
	int32 startx;
	int32 starty;
	int32 randomTargetPosition;
	if (parkingSpot1.xLoc <= 4)
	{
		startx = 1;
	}
	else
	{
		startx = 5;
	}
	if (parkingSpot1.yLoc <= 3)
	{
		starty = 1;
	}
	else
	{
		starty = 4;
	}

	for (int32 x = startx; x < startx + (board->GetBoardLength() - 2) / 2; x++)
	{
		for (int32 y = starty; y < starty + (board->GetBoardWidth() - 2) / 2; y++)
		{
			if (board)
			{
				if (board->GetboardTiles()[x * board->GetBoardLength() + y]->GetPlacedFireFighters()->Num() > 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Unable to use fire engine. Some one is in the quadrant"));
					return;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Board has not initialize."));
			}
		}
	}
	// randomly choose a target fire 
	randomTargetPosition = FMath::RandRange(startx * board->GetBoardLength() + starty
		, (startx + (board->GetBoardLength() - 2) / 2 - 1) * board->GetBoardLength() + starty + (board->GetBoardWidth() - 2) / 2 - 1);
	ATile* targetTile = board->GetboardTiles()[randomTargetPosition];
	UE_LOG(LogTemp, Warning, TEXT("The target tile is %d"), randomTargetPosition);
	if (targetTile)
	{
		// extinguish fire
		targetTile->SetFireStatus(EFireStatus::Clear);
		targetTile->GetSmokeEffect()->Deactivate();
		targetTile->GetFireEffect()->Deactivate();
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("random Error"));
	}
	
	// splash over
	splashOver(targetTile, EDirection::Up);
	splashOver(targetTile, EDirection::Down);
	splashOver(targetTile, EDirection::Left);
	splashOver(targetTile, EDirection::Right);
}

// Called when the game starts or when spawned
void AFireEngine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFireEngine::splashOver(ATile * targetTile, EDirection direction)
{
	AEdgeUnit* adjacentWall = nullptr;
	switch (direction)
	{
	case EDirection::Down:
		adjacentWall = targetTile->GetBack();
		break;
	case EDirection::Left:
		adjacentWall = targetTile->GetLeft();
		break;
	case EDirection::Right:
		adjacentWall = targetTile->GetRight();
		break;
	case EDirection::Up:
		adjacentWall = targetTile->GetFront();
		break;
	}

	if (adjacentWall)
	{
		if (!adjacentWall->IsBlocked())
		{
			ATile* adjacentTile = adjacentWall->GetOtherNeighbour(targetTile);
			if (adjacentTile)
			{
				if (adjacentTile->GetFireStatus() != EFireStatus::Clear)
				{
					adjacentTile->SetFireStatus(EFireStatus::Clear);
					adjacentTile->SetFireEffect(false);
					adjacentTile->SetSmokeEffect(false);
				}
			}
		}
	}
}

void AFireEngine::SetFEPosition(int32 newPos)
{
	currentPosition = newPos;
}

int32 AFireEngine::GetFEPosition()
{
	return currentPosition;
}