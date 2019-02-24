// Fill out your copyright notice in the Description page of Project Settings.

#include "EdgeUnit.h"


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

// Called when the game starts or when spawned
void AEdgeUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEdgeUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

