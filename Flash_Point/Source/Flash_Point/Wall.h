// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdgeUnit.h"
#include "Engine/World.h"
#include "Wall.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API AWall : public AEdgeUnit
{
	GENERATED_BODY()

public:
	AWall();

	UPROPERTY(ReplicatedUsing=Rep_WallMesh, EditAnyWhere, Category = "SetUp")
	bool isChoped;

	// A function to chop wall on the server
	void ChopWall();
	// A function to update wall mesh only on client
	void UpdateWallMesh(bool chopedDown);

	virtual void Damage() override;
	virtual FEdgeSaveInfo SaveEdge() override;
	virtual void LoadEdge(FEdgeSaveInfo edgeInfo) override;

protected:
	

	// Overriding setting all lifetime replicates function
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	// override the is blocked rep notify
	UFUNCTION()
	virtual void Rep_BlockStatus() override;
	// A function to replicate wall mesh
	UFUNCTION()
	void Rep_WallMesh();
	// Cursor clicked method
	UFUNCTION()
	void OnWallClicked(AActor * Target, FKey ButtonPressed);
	// when cursor is over the actor
	UFUNCTION()
	void OnWallOver(UPrimitiveComponent* Component);
	// when cursor left the actor
	UFUNCTION()
	void OnWallLeft(UPrimitiveComponent* Component);

	virtual void BeginPlay() override;
	
	
};
