// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "EdgeUnit.h"
#include "GeneralTypes.h"
#include "Engine/World.h"
#include "Tile.generated.h"

UCLASS()
class FLASH_POINT_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// This method will change the tile's type together with its supposed base color
	void SetTileType(ETileType tileType);
	// Below are binding functions for edge units
	void BindFrontWall(AEdgeUnit* edge);
	void BindBackWall(AEdgeUnit* edge);
	void BindLeftWall(AEdgeUnit* edge);
	void BindRightWall(AEdgeUnit* edge);

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
	// Plane color for hidden quadrant view
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	UMaterialInterface* hiddenMat = nullptr;
	// Plane color for odd quadrant
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	UMaterialInterface* oddMat = nullptr;
	// Plane color for even quadrant
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	UMaterialInterface* evenMat = nullptr;
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

	// Other references and variables
	UMaterialInterface* baseMat = nullptr;
	ETileType type = ETileType::Default;

	// FUNCTIONS
	// Here is a function that binds all cursor functions
	void BindCursorFunc();
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
