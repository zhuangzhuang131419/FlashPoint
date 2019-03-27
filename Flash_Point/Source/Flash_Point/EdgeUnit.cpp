// Fill out your copyright notice in the Description page of Project Settings.

#include "EdgeUnit.h"
#include "GameBoard.h"


// Sets default values
AEdgeUnit::AEdgeUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the edge unit's mesh
	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("WallMesh"));
}

void AEdgeUnit::BindFirstNeighbour(ATile * firstTile)
{
	if (ensure(firstNeighbour)) {
		UE_LOG(LogTemp, Warning, TEXT("failed on binding existing first neighbour on wall: %s"), *GetName());
	}
	else {
		firstNeighbour = firstTile;
	}
}

void AEdgeUnit::BindSecondNeighbour(ATile * secondTile)
{
	if (ensure(secondNeighbour)) {
		UE_LOG(LogTemp, Warning, TEXT("failed on binding existing second neighbour on wall: %s"), *GetName());
	}
	else {
		secondNeighbour = secondTile;
	}
}

void AEdgeUnit::BindBoard(AGameBoard * board)
{
	gameBoard = board;
}

ATile * AEdgeUnit::GetOtherNeighbour(ATile * current)
{
	if (current == firstNeighbour) {
		return secondNeighbour;
	}
	else if(current == secondNeighbour) {
		return firstNeighbour;
	}
	else {
		return nullptr;
	}
}

void AEdgeUnit::Damage()
{
	WallMesh->SetVisibility(false);
	isBlocked = false;
}

FEdgeSaveInfo AEdgeUnit::SaveEdge()
{
	return FEdgeSaveInfo();
}

bool AEdgeUnit::IsBlocked()
{
	return isBlocked;
}

void AEdgeUnit::SetIsBlocked(bool blocked)
{
	isBlocked = blocked;
}

EEdgeType AEdgeUnit::GetEdgeType()
{
	return edgeType;
}

UStaticMeshComponent * AEdgeUnit::GetWallMesh()
{
	return WallMesh;
}

void AEdgeUnit::Rep_BlockStatus()
{
	// not really doing anything here since edge unit's isblocked status is not likely to change
}

void AEdgeUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEdgeUnit, firstNeighbour);
	DOREPLIFETIME(AEdgeUnit, secondNeighbour);
	DOREPLIFETIME(AEdgeUnit, isBlocked);
	DOREPLIFETIME(AEdgeUnit, edgeType);
	DOREPLIFETIME(AEdgeUnit, gameBoard);
	DOREPLIFETIME(AEdgeUnit, WallMesh);
}

// Called when the game starts or when spawned
void AEdgeUnit::BeginPlay()
{
	// replicate this object
	if (HasAuthority()) {
		SetReplicates(true);
	}

	Super::BeginPlay();
	
}

// Called every frame
void AEdgeUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}