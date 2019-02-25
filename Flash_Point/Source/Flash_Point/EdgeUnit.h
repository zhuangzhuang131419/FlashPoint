// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneralTypes.h"
#include "EdgeUnit.generated.h"

class ATile;

UCLASS()
class FLASH_POINT_API AEdgeUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEdgeUnit();

	// Methods to bind neighbour walls
	void BindFirstNeighbour(ATile* firstTile);
	void BindSecondNeighbour(ATile* secondTile);
	// A blueprint event to open the door
	UFUNCTION(BlueprintImplementableEvent, Category = "Edge Utilities")
	void OnOpenDoor();

protected:
	// FIELDS
	// neighbouring tiles of the edge
	ATile* firstNeighbour = nullptr;
	ATile* secondNeighbour = nullptr;
	// informations about the edge unit
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	bool isBlocked = false;
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	EEdgeType edgeType = EEdgeType::Empty;
	// Here are the neighbouring tiles of the edge
	UPROPERTY(EditAnyWhere, Category = "Map Associations")
	ATile* tile_a = nullptr;
	UPROPERTY(EditAnyWhere, Category = "Map Associations")
	ATile* tile_b = nullptr; 
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	UStaticMeshComponent* WallMesh = nullptr;

	// FUNCTIONS
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
