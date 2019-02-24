// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "EdgeUnit.h"
#include "Engine/World.h"
#include "Tile.generated.h"

UCLASS()
class FLASH_POINT_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// Here is a function that binds all cursor functions
	void BindCursorFunc();

protected:
	// FIELDS

	// Static mesh components
	// A Static mesh component to store the floor mesh
	UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent* TileMesh = nullptr;
	// A static mesh to store the color plane
	UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent* ColorPlane = nullptr;

	// Color mat components
	// Default color of the plane goes here
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	UMaterialInterface* baseMat = nullptr;
	// Here is the walkable color
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	UMaterialInterface* ableMat = nullptr;
	// Here is the un-walkable color
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	UMaterialInterface* unableMat = nullptr;
	// Here is the fire engine parking lot color
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	UMaterialInterface* engineParkMat = nullptr;
	// Here is the ambulance parking lot color
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	UMaterialInterface* ambulanceParkMat = nullptr;

	// Edge class components
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AEdgeUnit> EdgeClass = nullptr;	// for regular edge class
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AEdgeUnit> WallClass = nullptr;	// for wall edge class
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AEdgeUnit> DoorClass = nullptr;	// for door edge class

	// Neighbour edge refences
	UPROPERTY(EditAnyWhere, Category = "Map Associations")
	AEdgeUnit* leftWall = nullptr;
	UPROPERTY(EditAnyWhere, Category = "Map Associations")
	AEdgeUnit* rightWall = nullptr;
	UPROPERTY(EditAnyWhere, Category = "Map Associations")
	AEdgeUnit* frontWall = nullptr;
	UPROPERTY(EditAnyWhere, Category = "Map Associations")
	AEdgeUnit* backWall = nullptr;

	// FUNCTIONS
	// Cursor over method implementation
	UFUNCTION()
	void OnCursorOver(UPrimitiveComponent* Component);
	// Cursor clicked method
	UFUNCTION()
	void OnCursorClicked(UPrimitiveComponent* Component);
	// Cursor leave method
	UFUNCTION()
	void OnCursorLeft(UPrimitiveComponent* Component);
	// A method to set material of the plane
	void PlaneColorSwitch(UMaterialInterface* mat);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
