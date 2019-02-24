// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Intialize the floor and plane objects
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Tile Mesh"));
	ColorPlane = CreateDefaultSubobject<UStaticMeshComponent>(FName("Color Plane"));
	ColorPlane->AttachToComponent(TileMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName("Plane"));

}

void ATile::SetTileType(ETileType tileType)
{
	if (tileType == ETileType::AmbulancePark) {
		type = tileType;
		baseMat = ambulanceParkMat;
		PlaneColorSwitch(baseMat);
	}
	else if (tileType == ETileType::FireEnginePark) {
		type = tileType;
		baseMat = engineParkMat;
		PlaneColorSwitch(baseMat);
	}
}

void ATile::SetQuadrant(int32 quad)
{
	// set quadrant and color of the tile
	quadrant = quad;
	if (quadrant % 2) {
		baseMat = oddMat;
		PlaneColorSwitch(baseMat);
	}
	else {
		baseMat = evenMat;
		PlaneColorSwitch(baseMat);
	}
}

AEdgeUnit * ATile::BuildEdgeRight(int32 type)
{
	// return null if proper classes are not available
	if (!ensure(EdgeClass) || !ensure(WallClass) || !ensure(DoorClass)) {
		return nullptr;
	}

	// Initialize specified edge units on the right side of the tile
	if (type == 0) {
		// build an empty edge
		return GetWorld()->SpawnActor<AEdgeUnit>(
			EdgeClass,
			TileMesh->GetSocketLocation(FName("WallRight")),
			TileMesh->GetSocketRotation(FName("WallRight"))
			);
	}
	else if (type == 1) {
		// build a wall edge
		return GetWorld()->SpawnActor<AEdgeUnit>(
			WallClass,
			TileMesh->GetSocketLocation(FName("WallRight")),
			TileMesh->GetSocketRotation(FName("WallRight"))
			);
	}
	else if (type == 2) {
		// build a door edge
		return GetWorld()->SpawnActor<AEdgeUnit>(
			DoorClass,
			TileMesh->GetSocketLocation(FName("WallRight")),
			TileMesh->GetSocketRotation(FName("WallRight"))
			);
	}
	else {
		return nullptr;
	}
}

AEdgeUnit * ATile::BuildEdgeFront(int32 type)
{
	// return null if proper classes are not available
	if (!ensure(EdgeClass) || !ensure(WallClass) || !ensure(DoorClass)) {
		return nullptr;
	}

	// Initialize specified edge units on the front side of the tile
	if (type == 0) {
		// build an empty edge
		return GetWorld()->SpawnActor<AEdgeUnit>(
			EdgeClass,
			TileMesh->GetSocketLocation(FName("WallFront")),
			TileMesh->GetSocketRotation(FName("WallFront"))
			);
	}
	else if (type == 1) {
		// build a wall edge
		return GetWorld()->SpawnActor<AEdgeUnit>(
			WallClass,
			TileMesh->GetSocketLocation(FName("WallFront")),
			TileMesh->GetSocketRotation(FName("WallFront"))
			);
	}
	else if (type == 2) {
		// build a door edge
		return GetWorld()->SpawnActor<AEdgeUnit>(
			DoorClass,
			TileMesh->GetSocketLocation(FName("WallFront")),
			TileMesh->GetSocketRotation(FName("WallFront"))
			);
	}
	else {
		return nullptr;
	}
}

void ATile::BindFrontEdge(AEdgeUnit * edge)
{
	// if a front wall already exist, delete it
	if (ensure(frontWall)) {
		frontWall->Destroy();
	}
	frontWall = edge;
	if (ensure(frontWall)) {
		frontWall->BindSecondNeighbour(this);
	}
}

void ATile::BindBackEdge(AEdgeUnit * edge)
{
	// if a back wall already exist, delete it
	if (ensure(backWall)) {
		backWall->Destroy();
	}
	backWall = edge;
	if (ensure(backWall)) {
		backWall->BindFirstNeighbour(this);
	}
}

void ATile::BindLeftEdge(AEdgeUnit * edge)
{
	// if a left wall already exist, delete it
	if (ensure(leftWall)) {
		leftWall->Destroy();
	}
	leftWall = edge;
	if (ensure(leftWall)) {
		leftWall->BindSecondNeighbour(this);
	}
}

void ATile::BindRightEdge(AEdgeUnit * edge)
{
	// if a right wall already exist, delete it
	if (ensure(rightWall)) {
		rightWall->Destroy();
	}
	rightWall = edge;
	if (ensure(rightWall)) {
		rightWall->BindFirstNeighbour(this);
	}
}

// Here is the function to bind all input bindings
void ATile::BindCursorFunc()
{
	// Create binding to on cursor over
	FScriptDelegate onMouseOverDel;
	onMouseOverDel.BindUFunction(this, "OnCursorOver");
	OnBeginCursorOver.Add(onMouseOverDel);

	// Create binding to on cursor click
	FScriptDelegate onMouseClickedDel;
	onMouseClickedDel.BindUFunction(this, "OnCursorClicked");
	OnClicked.Add(onMouseClickedDel);

	// Create binding to on cursor leave
	FScriptDelegate onMouseLeftDel;
	onMouseLeftDel.BindUFunction(this, "OnCursorLeft");
	OnEndCursorOver.Add(onMouseLeftDel);
}

void ATile::OnCursorOver(UPrimitiveComponent * Component)
{
	PlaneColorSwitch(ableMat);
	UE_LOG(LogTemp, Warning, TEXT("Mouse Over"));
}

void ATile::OnCursorClicked(UPrimitiveComponent* Component)
{
	UE_LOG(LogTemp, Warning, TEXT("Mouse Clicked"));
}

void ATile::OnCursorLeft(UPrimitiveComponent * Component)
{
	PlaneColorSwitch(baseMat);
	UE_LOG(LogTemp, Warning, TEXT("Mouse Left"));
}

void ATile::PlaneColorSwitch(UMaterialInterface * mat)
{
	if (ensure(ColorPlane)) {
		ColorPlane->SetMaterial(0, mat);
	}
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
	BindCursorFunc();

	// Assign temporary basic mat
	baseMat = hiddenMat;
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

