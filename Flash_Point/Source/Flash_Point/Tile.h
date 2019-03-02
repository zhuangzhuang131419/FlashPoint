// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "EdgeUnit.h"
#include "FireFighterPawn.h"
#include "FPPlayerController.h"
#include "Particles/ParticleSystemComponent.h"
#include "GeneralTypes.h"
#include "Engine/World.h"
#include "Tile.generated.h"

class AGameBoard;

UCLASS()
class FLASH_POINT_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// This method will change the tile's type together with its supposed base color
	void SetTileType(ETileType tileType);
	// This method will set the quarant and therefore default color of the tile
	void SetQuadrant(int32 quad);
	// Getter and setter of the location of the tile
	void SetLocation(int32 x, int32 y);
	bool GetLocation(int32& x, int32& y);
	// Methods for creating and returning edge units with respect to current tile, 0 for default, 1 for wall, 2 for door
	AEdgeUnit* BuildEdgeRight(int32 type);
	AEdgeUnit* BuildEdgeFront(int32 type);
	// Below are binding functions for edge units
	void BindFrontEdge(AEdgeUnit* edge);
	void BindBackEdge(AEdgeUnit* edge);
	void BindLeftEdge(AEdgeUnit* edge);
	void BindRightEdge(AEdgeUnit* edge);
	// check if the tile is a outside tile
	UFUNCTION(BlueprintCallable, Category = "Tile Attributes")
	bool IsOutside();
	// Getter and setter for fire status
	UFUNCTION(BlueprintCallable, Category = "Tile Attributes")
	EFireStatus GetFireStatus();
	UFUNCTION(BlueprintCallable, Category = "Tile Attributes")
	void SetFireStatus(EFireStatus status);


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
	// Plane color for hidden quadrant view and attribute indicating is outside
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
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setup")
	bool outside = false;	// for door edge class

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

	// particle systems for fire status
	UPROPERTY(VisibleAnyWhere, Category = "Fire Status")
	UParticleSystemComponent* FireEffect;
	UPROPERTY(VisibleAnyWhere, Category = "Fire Status")
	UParticleSystemComponent* SmokeEffect;
	UPROPERTY(VisibleAnyWhere, Category = "Fire Status")
	UParticleSystemComponent* BlastEffect;

	// located items and firefighters
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Tile Units")
	TArray<AFireFighterPawn*> placedFireFighters;

	// Other references and variables
	UMaterialInterface* baseMat = nullptr;	// the default color of the tile
	ETileType type = ETileType::Default;	// the default type of the tile
	EFireStatus fireStatus = EFireStatus::Clear;	// the default status of the tile
	int32 quadrant = 0;	// default quarant of the tile
	AFPPlayerController* localPlayer = nullptr;
	AFireFighterPawn* localPawn = nullptr;
	
	// attributes to use for path finding
	ATile* prev = nullptr;	// a pointer to follow for path finding
	int32 xLoc, yLoc = -1;	// location of the tile, to be specified with resonable value at instantiation
	bool canMoveTo = false;
	bool isReady = false;

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
	// A method to find path to current tile from player pawn's tile
	void FindPathToCurrent();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
