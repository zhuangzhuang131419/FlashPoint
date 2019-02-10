// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EdgeUnit.generated.h"

class ATile;

UCLASS()
class FLASH_POINT_API AEdgeUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEdgeUnit();

protected:
	// Here are the neighbouring tiles of the edge
	UPROPERTY(EditAnyWhere, Category = "Map Associations")
	ATile* tile_a = nullptr;
	UPROPERTY(EditAnyWhere, Category = "Map Associations")
	ATile* tile_b = nullptr; UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent* WallMesh = nullptr;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
