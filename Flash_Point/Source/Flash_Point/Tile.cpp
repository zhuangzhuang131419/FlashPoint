// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "GameBoard.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Intialize the floor and plane objects
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Tile Mesh"));
	ColorPlane = CreateDefaultSubobject<UStaticMeshComponent>(FName("Color Plane"));
	ColorPlane->AttachToComponent(TileMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName("Plane"));

	// Initialize all fire status effects
	FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(FName("Fire Effect"));
	FireEffect->bAutoActivate = false;
	FireEffect->AttachToComponent(TileMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName("VisualEffects"));
	SmokeEffect = CreateDefaultSubobject<UParticleSystemComponent>(FName("Smoke Effect"));
	SmokeEffect->bAutoActivate = false;
	SmokeEffect->AttachToComponent(TileMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName("VisualEffects"));
	BlastEffect = CreateDefaultSubobject<UParticleSystemComponent>(FName("Blast Effect"));
	BlastEffect->bAutoActivate = false;
	BlastEffect->AttachToComponent(TileMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName("VisualEffects"));

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

void ATile::LinkGameBoard(AGameBoard * gameBoard)
{
	board = gameBoard;
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

void ATile::SetLocation(int32 x, int32 y)
{
	xLoc = x;
	yLoc = y;
}

bool ATile::GetLocation(int32 & x, int32 & y)
{
	x = xLoc;
	y = yLoc;
	if (x >= 0 && y >= 0) {
		return true;
	}
	else {
		return false;
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

AEdgeUnit * ATile::GetFront()
{
	return frontWall;
}

AEdgeUnit * ATile::GetBack()
{
	return backWall;
}

AEdgeUnit * ATile::GetLeft()
{
	return leftWall;
}

AEdgeUnit * ATile::GetRight()
{
	return rightWall;
}

ATile * ATile::GetPrev()
{
	return prev;
}

void ATile::SetPrev(ATile * prevTile)
{
	prev = prevTile;
}

bool ATile::IsExpanded()
{
	return expanded;
}

void ATile::SetExpanded(bool exp)
{
	expanded = exp;
}

int32 ATile::GetPathCost()
{
	return pathCost;
}

void ATile::SetpathCost(int32 cost)
{
	pathCost = cost;
}

void ATile::ResetTile()
{
	PlaneColorSwitch(baseMat);
	prev = nullptr;
	expanded = false;
	pathCost = -1;
}

bool ATile::IsOutside()
{
	return outside;
}

EFireStatus ATile::GetFireStatus()
{
	return fireStatus;
}

void ATile::SetFireStatus(EFireStatus status)
{
	fireStatus = status;
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
	UE_LOG(LogTemp, Warning, TEXT("Mouse Over"));
	if (ensure(localPlayer)) {
		EGameOperations ops = localPlayer->GetCurrentOperation();
		switch (ops)
		{
		case EGameOperations::PlaceFireFighter:
			if (outside) {
				PlaneColorSwitch(ableMat);
			}
			else {
				PlaneColorSwitch(unableMat);
			}
			break;
		case EGameOperations::RespawnFireFighter:
			break;
		case EGameOperations::Move:
			FindPathToCurrent();
			break;
		case EGameOperations::ChopWall:
			break;
		case EGameOperations::ExtinguishFire:
			break;
		case EGameOperations::Carry:
			break;
		case EGameOperations::OpenDoor:
			break;
		case EGameOperations::None:
			break;
		default:
			break;
		}
	}
}

void ATile::OnCursorClicked(UPrimitiveComponent* Component)
{
	if (ensure(localPlayer)) {
		EGameOperations ops = localPlayer->GetCurrentOperation();
		switch (ops)
		{
		case EGameOperations::PlaceFireFighter:
			if (outside) {
				// Place firefighter to current tile
				localPlayer->GetPawn()->SetActorLocation(TileMesh->GetSocketLocation("VisualEffects"));
				localPlayer->SetNone();
				// Add the firefighter to the current position actors
				if (ensure(localPawn)) {
					// Associate the firefighter with this tile
					placedFireFighters.Add(localPawn);
					localPawn->SetPlacedOn(this);
				}
			}
			break;
		case EGameOperations::RespawnFireFighter:
			break;
		case EGameOperations::Move:
			if (isReady && canMoveTo) {
				localPlayer->GetPawn()->SetActorLocation(TileMesh->GetSocketLocation("VisualEffects"));
				localPlayer->SetNone();
				if (ensure(localPawn)) {
					// Associate the firefighter with this tile
					placedFireFighters.Add(localPawn);
					localPawn->SetPlacedOn(this);
				}
			}
			break;
		case EGameOperations::ChopWall:
			break;
		case EGameOperations::ExtinguishFire:
			break;
		case EGameOperations::Carry:
			break;
		case EGameOperations::OpenDoor:
			break;
		case EGameOperations::None:
			break;
		default:
			break;
		}
	}
}

void ATile::OnCursorLeft(UPrimitiveComponent * Component)
{
	board->ClearAllTile();
	UE_LOG(LogTemp, Warning, TEXT("Mouse Left"));
	// Reset move related attributes
	isReady = false;
	canMoveTo = false;
}

void ATile::PlaneColorSwitch(UMaterialInterface * mat)
{
	if (ensure(ColorPlane)) {
		ColorPlane->SetMaterial(0, mat);
	}
}

void ATile::FindPathToCurrent()
{
	if (!ensure(localPawn))	return;
	// Do an a* search to find the shortest path to this target location
	TArray<ATile*> traceTiles;
	ATile* start = localPawn->GetPlacedOn();
	ATile* goal = this;
	UE_LOG(LogTemp, Warning, TEXT("Before search"));
	int32 cost = GeneralTypes::AStarShotest(localPawn->GetCurrentAP(), start, goal, traceTiles);
	if (cost != 0) {
		// here the goal is successfully found
		if (cost <= localPawn->GetCurrentAP()) {
			canMoveTo = true;
			for (int32 i = traceTiles.Num() - 1; i >= 0; i--) {
				traceTiles[i]->PlaneColorSwitch(ableMat);
			}
		}
		else {
			for (int32 i = traceTiles.Num() - 1; i >= 0; i--) {
				traceTiles[i]->PlaneColorSwitch(unableMat);
			}
		}
		isReady = true;
	}
	else {
		if (traceTiles.Num() > 0) {
			canMoveTo = true;
			PlaneColorSwitch(ableMat);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("After search"));
}

void ATile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// super first
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// mark all tile attributes as replicate to only do things on server
	DOREPLIFETIME(ATile, leftWall);
	DOREPLIFETIME(ATile, rightWall);
	DOREPLIFETIME(ATile, frontWall);
	DOREPLIFETIME(ATile, backWall);
	DOREPLIFETIME(ATile, placedFireFighters);
	DOREPLIFETIME(ATile, baseMat);
	DOREPLIFETIME(ATile, quadrant);
	DOREPLIFETIME(ATile, board);
	DOREPLIFETIME(ATile, xLoc);
	DOREPLIFETIME(ATile, yLoc);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		SetReplicates(true);
	}
	BindCursorFunc();	

	// Find the local player and local pawn
	localPlayer = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
	localPawn = Cast<AFireFighterPawn>(localPlayer->GetPawn());
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

