// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "GameBoard.h"
#include "Victim.h"
#include "Hazmat.h"
#include "Wall.h"
#include "POI.h"
#include "Door.h"


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

void ATile::PawnMoveToHere(AFireFighterPawn* movingPawn, const TArray<ATile*>& trace)
{
	if (ensure(movingPawn)) {
		movingPawn->SetActorLocation(TileMesh->GetSocketLocation("VisualEffects"));
		// Associate the firefighter with this tile
		placedFireFighters.Add(movingPawn);
		movingPawn->GetPlacedOn()->placedFireFighters.Remove(movingPawn);
		movingPawn->SetPlacedOn(this);

		if (POIStatus == EPOIStatus::Hided)
		{
			UE_LOG(LogTemp, Warning, TEXT("A POI has been revealed."));
			if (!ensure(POIOnTile)) return;
			if (POIOnTile->isAlarm)
			{
				FVector VictimSocketLocation;
				switch (GetVictims()->Num())
				{
				case 0:
					VictimSocketLocation = GetTileMesh()->GetSocketLocation(FName("Victim"));
					break;
				case 1:
					VictimSocketLocation = GetTileMesh()->GetSocketLocation(FName("Victim1"));
					break;
				case 2:
					VictimSocketLocation = GetTileMesh()->GetSocketLocation(FName("Victim3"));
					break;
				case 3:
					VictimSocketLocation = GetTileMesh()->GetSocketLocation(FName("Victim4"));
					break;
				default:
					UE_LOG(LogTemp, Warning, TEXT("No more position"))
						VictimSocketLocation = GetTileMesh()->GetSocketLocation(FName("Victim"));
					break;
				}
				AVictim* newVictim = GetWorld()->SpawnActor<AVictim>(
					victimClass,
					VictimSocketLocation,
					FRotator(0, 0, 0)
					);
				if (ensure(newVictim))
				{
					victims.Add(newVictim);
				}
				POIStatus = EPOIStatus::Revealed;
			}
			else
			{
				POIStatus = EPOIStatus::Empty;
				board->SetCurrentPOI(board->currentPOI - 1);
			}
			POIOnTile->Destroy();
			POIOnTile = nullptr;			
		}
	}
}

void ATile::PlacePawnHere(AFireFighterPawn* placingPawn)
{
	if (ensure(placingPawn)) {
		// Add the firefighter to the current position actors
		placingPawn->SetActorLocation(TileMesh->GetSocketLocation("VisualEffects"));
		// Associate the firefighter with this tile
		placedFireFighters.Add(placingPawn);
		UE_LOG(LogTemp, Warning, TEXT("Pawn location has been set"));
		placingPawn->SetPlacedOn(this);
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

EPOIStatus ATile::GetPOIStatus()
{
	return POIStatus;
}

void ATile::SetPOIStatus(EPOIStatus status)
{
	POIStatus = status;
}

UParticleSystemComponent * ATile::GetFireEffect()
{
	return FireEffect;
}

UParticleSystemComponent * ATile::GetSmokeEffect()
{
	return SmokeEffect;
}

UParticleSystemComponent * ATile::GetBlastEffect()
{
	return BlastEffect;
}

void ATile::SetFireEffect(bool isActivate)
{
	if (isActivate)
	{
		FireEffect->Activate();
	}
	else
	{
		FireEffect->Deactivate();
	}
}

void ATile::SetSmokeEffect(bool isActivate)
{
	if (isActivate)
	{
		SmokeEffect->Activate();
	}
	else
	{
		SmokeEffect->Deactivate();
	}
}

APOI* ATile::GetPOIOnTile()
{
	return POIOnTile;
}

void ATile::SetPOIOnTile(APOI* inPOI)
{
	POIOnTile = inPOI;
}


UStaticMeshComponent * ATile::GetTileMesh()
{
	return TileMesh;
}

TArray<AVictim*>* ATile::GetVictims()
{
	return &victims;
}

AGameBoard * ATile::GetGameBoard()
{
	return board;
}

void ATile::AdvanceFire()
{
	if (HasAuthority()) {
		fireStatus = EFireStatus::Fire;
	}
	SmokeEffect->Deactivate();
	FireEffect->Activate();
}

void ATile::AdvancePOI()
{
	if (ensure(fireStatus != EFireStatus::Fire))
	{
		if (ensure(board->currentPOI < board->maxPOI))
		{
			if (ensure(!POIOnTile))
			{
				if (ensure(POIStatus == EPOIStatus::Empty))
				{
					UE_LOG(LogTemp, Warning, TEXT("Advance POI"));
					
					SetPOIStatus(EPOIStatus::Hided);
					FVector POISocketLocation = TileMesh->GetSocketLocation(FName("POI"));
					APOI* inPOI = GetWorld()->SpawnActor<APOI>(
						POIClass,
						POISocketLocation,
						FRotator(0, 0, 0)
						);
					if (board)
					{
						board->setPOIalarm(inPOI);
						board->SetCurrentPOI(board->currentPOI + 1);
						SetPOIOnTile(inPOI);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Wrong"));
					}
					
				}
			}
		}
	}
}

void ATile::SetSmokeOnTile()
{
	if (HasAuthority()) {
		fireStatus = EFireStatus::Smoke;
	}
	SmokeEffect->Activate();
	FireEffect->Deactivate();
}

void ATile::SetClearOnTile()
{
	if (HasAuthority()) {
		fireStatus = EFireStatus::Clear;
	}
	FireEffect->Deactivate();
	SmokeEffect->Deactivate();
}

void ATile::AdvanceExplosion(EDirection direction)
{
	AEdgeUnit* adjacentWall = nullptr;
	switch (direction) 
	{
		case EDirection::Down:
			adjacentWall = backWall;
			break;
		case EDirection::Left:
			adjacentWall = leftWall;
			break;
		case EDirection::Right:
			adjacentWall = rightWall;
			break;
		case EDirection::Up:
			adjacentWall = frontWall;
			break;
	}

	if (adjacentWall)
	{
		if (adjacentWall->IsBlocked())
		{			
			if (ensure(adjacentWall->GetEdgeType() == EEdgeType::Wall))
			{
				if (Cast<AWall>(adjacentWall)->isChoped)
				{
					(Cast<AWall>(adjacentWall))->Damage();
				}
				else {
					Cast<AWall>(adjacentWall)->ChopWall();
				}
			}
			else if (adjacentWall->GetEdgeType() == EEdgeType::Door) {
				(Cast<ADoor>(adjacentWall))->Damage();
			}
		}
		else
		{
			if (adjacentWall->GetEdgeType() == EEdgeType::Door) {
				(Cast<ADoor>(adjacentWall))->Damage();
			}
			if (adjacentWall->GetOtherNeighbour(this))
			{
				if (adjacentWall->GetOtherNeighbour(this)->GetFireStatus() != EFireStatus::Fire)
				{
					if (adjacentWall->GetOtherNeighbour(this)->GetFireStatus() == EFireStatus::Smoke)
					{
						adjacentWall->GetOtherNeighbour(this)->FireEffect->Activate();
						adjacentWall->GetOtherNeighbour(this)->SmokeEffect->Deactivate();
					}
					else
					{
						adjacentWall->GetOtherNeighbour(this)->FireEffect->Activate();
					}
					adjacentWall->GetOtherNeighbour(this)->SetFireStatus(EFireStatus::Fire);
				}
				else
				{
					adjacentWall->GetOtherNeighbour(this)->AdvanceExplosion(direction);
				}
			}
		}
	}
}

void ATile::AdvanceExplosion()
{
	blastOccured = !blastOccured;
	AdvanceExplosion(EDirection::Down);
	AdvanceExplosion(EDirection::Left);
	AdvanceExplosion(EDirection::Right);
	AdvanceExplosion(EDirection::Up);
}

/*
return: true if the fire status not change
*/
bool ATile::Flashover(EDirection direction)
{
	AEdgeUnit* adjacentWall = nullptr;
	switch (direction)
	{
	case EDirection::Down:
		adjacentWall = backWall;
		break;
	case EDirection::Left:
		adjacentWall = leftWall;
		break;
	case EDirection::Right:
		adjacentWall = rightWall;
		break;
	case EDirection::Up:
		adjacentWall = frontWall;
		break;
	}

	if (adjacentWall)
	{
		if (!adjacentWall->IsBlocked())
		{
			ATile* adjacentTile = adjacentWall->GetOtherNeighbour(this);
			if (adjacentTile)
			{
				if (adjacentTile->GetFireStatus() == EFireStatus::Fire)
				{
					this->SetFireStatus(EFireStatus::Fire);
					this->FireEffect->Activate();
					this->SmokeEffect->Deactivate();
					return false;
				}
			}
		}
	}
	return true;
}

bool ATile::Flashover()
{
	bool result1 = true;
	bool result2 = true;
	bool result3 = true;
	bool result4 = true;
	if (GetFireStatus() == EFireStatus::Smoke) 
	{
		result1 = Flashover(EDirection::Down);
		result2 = Flashover(EDirection::Up);
		result3 = Flashover(EDirection::Left);
		result4 = Flashover(EDirection::Right);
	}
	return result1 && result2 && result3 && result4;
}

// Here is the function to bind all input bindings
void ATile::BindCursorFunc()
{
	// Create binding to on cursor over
	FScriptDelegate onMouseOverDel;
	onMouseOverDel.BindUFunction(this, "OnTileOver");
	OnBeginCursorOver.Add(onMouseOverDel);

	// Create binding to on cursor click
	/*FScriptDelegate onMouseClickedDel;
	onMouseClickedDel.BindUFunction(this, "OnTileClicked");*/
	OnClicked.AddUniqueDynamic(this, &ATile::OnTileClicked);

	// Create binding to on cursor leave
	FScriptDelegate onMouseLeftDel;
	onMouseLeftDel.BindUFunction(this, "OnTileLeft");
	OnEndCursorOver.Add(onMouseLeftDel);
}

void ATile::OnTileOver(UPrimitiveComponent * Component)
{
	// UE_LOG(LogTemp, Warning, TEXT("Mouse Over"));
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
			if (!ensure(localPawn)) return;
			// check if the player can extinguish fire on this tile
			if (AdjacentToPawn()) {
				// switch on fire status to set the tile color
				switch (fireStatus)
				{
				case EFireStatus::Clear:
					// no fire or smoke on the position, unable to perform extinguish
					PlaneColorSwitch(unableMat);
					break;
				case EFireStatus::Smoke:
					// smoke here, check if ap enough to extinguish it
					if (localPawn->CheckCanExtinguish(1)) { PlaneColorSwitch(ableMat); }
					else { PlaneColorSwitch(unableMat); }
					break;
				case EFireStatus::Fire:
					// check if the fire can be extinguished to nothing or just can be to smoke
					if (localPawn->CheckCanExtinguish(2)) { PlaneColorSwitch(ableMat); }
					else if (localPawn->CheckCanExtinguish(1)) { PlaneColorSwitch(halfExtinguishableMat); }
					else { PlaneColorSwitch(unableMat); }
					break;
				default:
					break;
				}
			}
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

void ATile::ExitinguishFireOnTile()
{
	// if the ap is not enough, return
	if (fireStatus == EFireStatus::Fire)
	{
		if (HasAuthority()) {
			SetSmokeOnTile();
		}
	}
	else if (fireStatus == EFireStatus::Smoke)
	{
		if (HasAuthority()) {
			SetClearOnTile();
		}
	}
}

void ATile::OnTileClicked(AActor* Target, FKey ButtonPressed)
{
	if (ButtonPressed != FKey("LeftMouseButton")) return;
	UE_LOG(LogTemp, Warning, TEXT("A tile has been clicked."));
	if (ensure(localPlayer)) {
		EGameOperations ops = localPlayer->GetCurrentOperation();
		switch (ops)
		{
		case EGameOperations::PlaceFireFighter:			
			if (outside) {
				localPlayer->SetNone();
				// Place firefighter to current tile
				localPawn = Cast<AFireFighterPawn>(localPlayer->GetPawn());
				if (ensure(localPawn))
				{
					if (HasAuthority()) {
						PlacePawnHere(localPawn);
						if (ensure(board)) {
							board->InitialPlacing();
						}
					}
					else {
						localPlayer->ServerPlacePawn(this, localPawn);
						// to make the placing visible to operation client
						localPawn->SetActorLocation(TileMesh->GetSocketLocation("VisualEffects"));
					}
				}
			}
			break;
		case EGameOperations::RespawnFireFighter:
			break;
		case EGameOperations::Move:
			// Move operations are done below
			if (isReady && canMoveTo) {
				if (!ensure(localPlayer)) return;
				if (!ensure(localPawn)) return;
				localPlayer->SetNone();
				if (HasAuthority()) {
					PawnMoveToHere(localPawn, pathToHere);
				}
				else {
					localPlayer->ServerMovePawn(this, localPawn, pathToHere);
					// to make the placing visible to operation client
					localPawn->SetActorLocation(TileMesh->GetSocketLocation("VisualEffects"));
				}
				// do ap adjustment
				if (localPawn->GetFireFighterRole() != ERoleType::RescueSpecialist)
				{
					localPawn->AdjustFireFighterAP(-costToHere);
				}
				else
				{
					localPawn->AdjustSpecialistMoveAP(-costToHere);
				}
				
				canMoveTo = false;
				isReady = false;
				costToHere = 0;
			}

			break;
		case EGameOperations::ChopWall:
			break;
		case EGameOperations::ExtinguishFire:
			// check if the fire is adjacent
			if (!AdjacentToPawn()) return;
			if (!ensure(localPawn)) return;
			if (localPawn->GetCurrentAP() + localPawn->GetExtinguishAP() < localPawn->GetExtinguishConsumption()) return;
			if (fireStatus == EFireStatus::Clear) return;
			if (localPawn->GetFireFighterRole() != ERoleType::CAFSFirefighter) 
			{
				localPawn->AdjustFireFighterAP(-localPawn->GetExtinguishConsumption());
			}
			else
			{
				localPawn->AdjustCAFSFireFighterExtinguishAP(-localPawn->GetExtinguishConsumption());
			}
			if (HasAuthority()) {
				ExitinguishFireOnTile();
			}
			else {
				localPlayer->ServerExtinguishFire(this);
			}
			break;
		case EGameOperations::Carry:
			break;
		case EGameOperations::OpenDoor:
			break;
		case EGameOperations::FireDeckGun:
			break;
		case EGameOperations::FlipPOI:
			if (!ensure(localPawn)) return;
			if (!ensure(localPlayer)) return;
			localPlayer->ServerRevealPOI(this);
			break;
		case EGameOperations::None:
			break;
		default:
			break;
		}
	}
}

void ATile::OnTileLeft(UPrimitiveComponent * Component)
{
	board->ClearAllTile();
	// UE_LOG(LogTemp, Warning, TEXT("Mouse Left"));
	// Reset move related attributes
	isReady = false;
	canMoveTo = false;
	costToHere = 0;
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
	// check if the player is carrying a victim and the trace has fire
	bool hasFire = false;
	for (ATile* traceTile : traceTiles) {
		if (traceTile->GetFireStatus() == EFireStatus::Fire) {
			hasFire = true;
			break;
		}
	}
	if (cost != 0) {
		// if the firefighter is carrying some victim, the cost is doubled
		cost = cost * localPawn->GetMoveConsumption();
		if (localPawn->GetVictim()) {
			// if carrying any victim, double the cost
			cost = cost * 2;
		}
		// here is the case if the firefighter is carring some victim but the trace has fire
		if (hasFire && localPawn->GetVictim()) {
			for (int32 i = traceTiles.Num() - 1; i >= 0; i--) {
				traceTiles[i]->PlaneColorSwitch(unableMat);
			}
		}
		// here the goal is successfully found and could be moved to
		else if (cost <= localPawn->GetCurrentAP() + localPawn->GetMovementAP()) {
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
	// change cost to here
	costToHere = cost;
	UE_LOG(LogTemp, Warning, TEXT("After search"));
}

bool ATile::AdjacentToPawn()
{
	if (ensure(localPawn)) {
		// check if the pawn is on the tile
		if (localPawn->GetPlacedOn() == this) {
			return true;
		}
		// check if the pawn is around the tile
		ATile* tempTile = nullptr;
		// check on front tile
		if (frontWall && !frontWall->IsBlocked()) {			
			tempTile = frontWall->GetOtherNeighbour(this);
			if (ensure(tempTile)) {
				if (tempTile->placedFireFighters.Contains(localPawn)) {
					return true;
				}
			}
		}
		// check on back tile
		if (backWall && !backWall->IsBlocked()) {
			tempTile = backWall->GetOtherNeighbour(this);
			if (ensure(tempTile)) {
				if (tempTile->placedFireFighters.Contains(localPawn)) {
					return true;
				}
			}
		}
		// check on left tile
		if (leftWall && !leftWall->IsBlocked()) {
			tempTile = leftWall->GetOtherNeighbour(this);
			if (ensure(tempTile)) {
				if (tempTile->placedFireFighters.Contains(localPawn)) {
					return true;
				}
			}
		}
		// check on right tile
		if (rightWall && !rightWall->IsBlocked()) {
			tempTile = rightWall->GetOtherNeighbour(this);
			if (ensure(tempTile)) {
				if (tempTile->placedFireFighters.Contains(localPawn)) {
					return true;
				}
			}
		}
	}
	return false;
}

void ATile::Rep_BaseMat()
{
	if (ensure(baseMat)) {
		PlaneColorSwitch(baseMat);
	}
}

void ATile::Rep_FireStatus()
{
	if (!ensure(FireEffect) || !ensure(SmokeEffect) || !ensure(BlastEffect))	return;
	switch (fireStatus)
	{
	case EFireStatus::Clear:
		SetClearOnTile();
		break;
	case EFireStatus::Smoke:
		SetSmokeOnTile();
		break;
	case EFireStatus::Fire:
		FireEffect->Activate();
		SmokeEffect->Deactivate();
		break;
	default:
		break;
	}
}

void ATile::Rep_BlastEffect()
{
	BlastEffect->DeactivateSystem();
	BlastEffect->ActivateSystem();
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
	DOREPLIFETIME(ATile, fireStatus);
	DOREPLIFETIME(ATile, victims);
	DOREPLIFETIME(ATile, POIOnTile);
	DOREPLIFETIME(ATile, POIStatus);
	DOREPLIFETIME(ATile, blastOccured);
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

