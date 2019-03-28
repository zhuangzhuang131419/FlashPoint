// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneralTypes.h"
#include "UnrealNetwork.h"
#include "EdgeUnit.generated.h"

class ATile;
class AGameBoard;

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
	// Method to bind the edge to current board for damage counting
	void BindBoard(AGameBoard* board);
	// A blueprint event to open the door
	UFUNCTION(BlueprintImplementableEvent, Category = "Edge Utilities")
	void OnOpenDoor();
	// Get other neighbour of the edge, used in move searching
	ATile* GetOtherNeighbour(ATile* current);

	virtual void Damage();
	virtual FEdgeSaveInfo SaveEdge();
	virtual void LoadEdge(FEdgeSaveInfo edgeInfo);

	// Getter and setter
	bool IsBlocked();
	void SetIsBlocked(bool blocked);

	// Getter and setter
	EEdgeType GetEdgeType();

	UStaticMeshComponent* GetWallMesh();

protected:
	// FIELDS
	// neighbouring tiles of the edge
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Associations")
	ATile* firstNeighbour = nullptr;
	UPROPERTY(replicated, EditAnyWhere, Category = "Map Associations")
	ATile* secondNeighbour = nullptr;
	// informations about the edge unit
	UPROPERTY(ReplicatedUsing=Rep_BlockStatus, VisibleAnyWhere, BlueprintReadWrite, Category = "Setup")
	bool isBlocked = false;
	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadWrite, Category = "Setup")
	EEdgeType edgeType = EEdgeType::Empty;
	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadOnly, Category = "Map Associations")
	AGameBoard* gameBoard = nullptr;
	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadWrite)
	UStaticMeshComponent* WallMesh = nullptr;
	// materials for indicating valid or invalid operations
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	UMaterialInterface* baseMat = nullptr;
	// Here is the do able color
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	UMaterialInterface* ableMat = nullptr;
	// Here stands for semi-doable
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	UMaterialInterface* semiAbleMat = nullptr;
	// Here is the unable color
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	UMaterialInterface* unableMat = nullptr;

	// FUNCTIONS
	// Function to update block related mesh view 
	UFUNCTION()
	virtual void Rep_BlockStatus();
	// mark all replicated properties
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
