// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "GameBoard.h"
#include "Victim.h"
#include "Hazmat.h"
#include "Ambulance.h"
#include "FireEngine.h"
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
	HotSpotEffect = CreateDefaultSubobject<UParticleSystemComponent>(FName("Hot Spot Effect"));
	HotSpotEffect->bAutoActivate = false;
	HotSpotEffect->AttachToComponent(TileMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName("VisualEffects"));
	SplashEffect = CreateDefaultSubobject<UParticleSystemComponent>(FName("Splash Effect"));
	SplashEffect->bAutoActivate = false;
	SplashEffect->AttachToComponent(TileMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName("VisualEffects"));

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
	if (leftWall) {
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
	if (rightWall) {
		rightWall->Destroy();
	}
	rightWall = edge;
	if (ensure(rightWall)) {
		rightWall->BindFirstNeighbour(this);
	}
}

void ATile::PlaceVictim()
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
		newVictim->SetPlacedOn(this);
		victims.Add(newVictim);
	}
	POIStatus = EPOIStatus::Revealed;
}

void ATile::PawnMoveToHere(AFireFighterPawn* movingPawn, const TArray<ATile*>& trace)
{
	if (ensure(movingPawn)) {
		movingPawn->SetActorLocation(TileMesh->GetSocketLocation("VisualEffects"));
		// Associate the firefighter with this tile
		placedFireFighters.Add(movingPawn);
		movingPawn->GetPlacedOn()->placedFireFighters.Remove(movingPawn);
		movingPawn->SetPlacedOn(this);

		// adjust the veteran position in gameboard 
		if (movingPawn->GetFireFighterRole() == ERoleType::Veteran)
		{
			board->SetVeteranLoc(this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Current fire fighter is placed on %s"), *movingPawn->GetPlacedOn()->GetName());
			if (movingPawn->CheckIsVicinty(board->GetVeteranLoc()))
			{
				movingPawn->SetDodgeAbility(true);
			}
		}

		if (movingPawn->GetLeading() != nullptr)
		{
			ATile* tempTile = movingPawn->GetLeading()->GetPlacedOn();
			if (ensure(tempTile))
			{
				tempTile->SetPOIStatus(EPOIStatus::Empty);
				movingPawn->GetLeading()->SetPlacedOn(this);
			}
			movingPawn->GetLeading()->SetActorLocation(movingPawn->GetActorLocation() - FVector(0, 100, 0));
			movingPawn->GetLeading()->SetVictimLoc(movingPawn->GetActorLocation() - FVector(0, 100, 0));
		}

		if (POIStatus == EPOIStatus::Hided)
		{
			UE_LOG(LogTemp, Warning, TEXT("A POI has been revealed."));
			if (!ensure(POIOnTile)) return;
			if (POIOnTile->isAlarm)
			{
				PlaceVictim();
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
		// if teh firefighter is veteran, just set it in board
		if (placingPawn->GetFireFighterRole() == ERoleType::Veteran) {
			if (ensure(board)) {
				board->SetVeteranLoc(this);
			}
		}
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
	// do not clear if this is command target
	if (!isCommandTarget) {
		PlaneColorSwitch(baseMat);
	}
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

void ATile::SetCommandTarget(bool isTarget)
{
	isCommandTarget = isTarget;
	if (isTarget) {
		PlaneColorSwitch(ableMat);
	}
	else {
		PlaneColorSwitch(baseMat);
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

void ATile::SetHazmatOnTile(AHazmat* inHazmat)
{
	HazmatOnTile = inHazmat;
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

void ATile::SpawnAmbulance()
{
	if (!frontWall) { SpawnAmbulance(EDirection::Up); }
	else if (!backWall) { SpawnAmbulance(EDirection::Down); }
	else if (!rightWall) { SpawnAmbulance(EDirection::Right); }
	else if (!leftWall) { SpawnAmbulance(EDirection::Left); }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnAmbulance Wrong"));
	}
}

void ATile::SpawnAmbulance(EDirection direction)
{
	FVector AmbulanceSocketLocation;
	FRotator AmbulanceSocketRotation;
	switch (direction)
	{
	case EDirection::Down:
		AmbulanceSocketLocation = TileMesh->GetSocketLocation(FName("VehicleBot"));
		AmbulanceSocketRotation = TileMesh->GetSocketRotation(FName("VehicleBot"));
		break;
	case EDirection::Up:
		AmbulanceSocketLocation = TileMesh->GetSocketLocation(FName("VehicleTop"));
		AmbulanceSocketRotation = TileMesh->GetSocketRotation(FName("VehicleTop"));
		break;
	case EDirection::Left:
		AmbulanceSocketLocation = TileMesh->GetSocketLocation(FName("VehicleLeft"));
		AmbulanceSocketRotation = TileMesh->GetSocketRotation(FName("VehicleLeft"));
		break;
	case EDirection::Right:
		AmbulanceSocketLocation = TileMesh->GetSocketLocation(FName("VehicleRight"));
		AmbulanceSocketRotation = TileMesh->GetSocketRotation(FName("VehicleRight"));
		break;
	default:
		// impossible
		break;
	}

	AAmbulance* ambulance = GetWorld()->SpawnActor<AAmbulance>(
		AmbulanceClass,
		AmbulanceSocketLocation,
		AmbulanceSocketRotation
		);
	if (ensure(ambulance))
	{
		ambulance->SetPlayingGameBoard(board);
		board->SetAmbulance(ambulance);
		// ambulance->SetAmbulancePosition(pos);
		// ambulance->RescueVictims(board->ambulanceLocA->GetVictims(), board->ambulanceLocA);
		// ambulance->RescueVictims(board->ambulanceLocB->GetVictims(), board->ambulanceLocB);
	}
}

void ATile::SpawnFireEngine()
{
	if (!frontWall) { SpawnFireEngine(EDirection::Up); }
	else if (!backWall) { SpawnFireEngine(EDirection::Down); }
	else if (!rightWall) { SpawnFireEngine(EDirection::Right); }
	else if (!leftWall) { SpawnFireEngine(EDirection::Left); }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnAmbulance Wrong"));
	}
}

void ATile::SpawnFireEngine(EDirection direction)
{
	FVector FireEngineSocketLocation;
	FRotator FireEngineSocketRotation;
	switch (direction)
	{
	case EDirection::Down:
		FireEngineSocketLocation = TileMesh->GetSocketLocation(FName("VehicleBot"));
		FireEngineSocketRotation = TileMesh->GetSocketRotation(FName("VehicleBot"));
		break;
	case EDirection::Up:
		FireEngineSocketLocation = TileMesh->GetSocketLocation(FName("VehicleTop"));
		FireEngineSocketRotation = TileMesh->GetSocketRotation(FName("VehicleTop"));
		break;
	case EDirection::Left:
		FireEngineSocketLocation = TileMesh->GetSocketLocation(FName("VehicleLeft"));
		FireEngineSocketRotation = TileMesh->GetSocketRotation(FName("VehicleLeft"));
		break;
	case EDirection::Right:
		FireEngineSocketLocation = TileMesh->GetSocketLocation(FName("VehicleRight"));
		FireEngineSocketRotation = TileMesh->GetSocketRotation(FName("VehicleRight"));
		break;
	default:
		// impossible
		break;
	}

	AFireEngine* fireEngine = GetWorld()->SpawnActor<AFireEngine>(
		FireEngineClass,
		FireEngineSocketLocation,
		FireEngineSocketRotation + FRotator(0, 90, 0)
		);

	if (ensure(fireEngine))
	{
		fireEngine->SetPlayingGameBoard(board);
		board->SetFireEngine(fireEngine);
		// ambulance->SetAmbulancePosition(pos);
	}
}

void ATile::AdvanceHazmat()
{
	if (ensure(fireStatus != EFireStatus::Fire))
	{
		if (ensure(!HazmatOnTile))
		{
			UE_LOG(LogTemp, Warning, TEXT("Advance Hazmat"));

			AHazmat* inHazmat = GetWorld()->SpawnActor<AHazmat>(
				HazmatClass,
				TileMesh->GetSocketLocation(FName("VisualEffects")),
				FRotator(0, 0, 0)
				);
			if (ensure(inHazmat))
			{
				inHazmat->SetHazmatLoc(inHazmat->GetActorLocation());
			}
			if (board)
			{
				SetHazmatOnTile(inHazmat);
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
			if (adjacentWall->GetEdgeType() == EEdgeType::Wall)
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
	if (ensure(board) && board->GetGameType() != EGameType::Family) {
		isHotSpot = true;
		if (ensure(HotSpotEffect)) {
			HotSpotEffect->ActivateSystem();
		}
	}
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

void ATile::setAmbulanceLocation(AAmbulance * currentAmbulance)
{
	AEdgeUnit* adjacentParkingWall = nullptr;
	if (ensure(type == ETileType::AmbulancePark))
	{
		if (ensure(adjacentParkTile))
		{
			adjacentParkingWall = isAdjacent(adjacentParkTile);
			if (ensure(adjacentParkingWall))
			{
				if (adjacentParkingWall == frontWall)
				{
					UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());
					if (!leftWall) { setAmbulanceLocation(currentAmbulance, EDirection::Left); }
					else { setAmbulanceLocation(currentAmbulance, EDirection::Right); }

				}
				else if (adjacentParkingWall == backWall)
				{
					UE_LOG(LogTemp, Warning, TEXT("%s"), *adjacentParkTile->GetName());
					if (!leftWall) { adjacentParkTile->setAmbulanceLocation(currentAmbulance, EDirection::Left); }
					else { adjacentParkTile->setAmbulanceLocation(currentAmbulance, EDirection::Right); }
				}
				else if (adjacentParkingWall == rightWall)
				{
					UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());
					if (!frontWall) { setAmbulanceLocation(currentAmbulance, EDirection::Up); }
					else { setAmbulanceLocation(currentAmbulance, EDirection::Down); }
				}
				else if (adjacentParkingWall == leftWall)
				{
					UE_LOG(LogTemp, Warning, TEXT("%s"), *adjacentParkTile->GetName());
					if (!frontWall) { adjacentParkTile->setAmbulanceLocation(currentAmbulance, EDirection::Up); }
					else { adjacentParkTile->setAmbulanceLocation(currentAmbulance, EDirection::Down); }
				}
			}
		}
	}
}

void ATile::setAmbulanceLocation(AAmbulance * currentAmbulance, EDirection direction)
{
	FVector AmbulanceSocketLocation;
	FRotator AmbulanceSocketRotation;
	switch (direction)
	{
	case EDirection::Down:
		AmbulanceSocketLocation = TileMesh->GetSocketLocation(FName("VehicleBot"));
		AmbulanceSocketRotation = TileMesh->GetSocketRotation(FName("VehicleBot"));
		break;
	case EDirection::Up:
		AmbulanceSocketLocation = TileMesh->GetSocketLocation(FName("VehicleTop"));
		AmbulanceSocketRotation = TileMesh->GetSocketRotation(FName("VehicleTop"));
		break;
	case EDirection::Left:
		AmbulanceSocketLocation = TileMesh->GetSocketLocation(FName("VehicleLeft"));
		AmbulanceSocketRotation = TileMesh->GetSocketRotation(FName("VehicleLeft"));
		break;
	case EDirection::Right:
		AmbulanceSocketLocation = TileMesh->GetSocketLocation(FName("VehicleRight"));
		AmbulanceSocketRotation = TileMesh->GetSocketRotation(FName("VehicleRight"));
		break;
	default:
		// impossible
		break;
	}
	if (ensure(adjacentParkTile))
	{
		if (ensure(board))
		{
			if (board->GetAmbulance())
			{
				board->GetAmbulance()->SetActorLocation(AmbulanceSocketLocation);
				board->GetAmbulance()->SetActorRotation(AmbulanceSocketRotation);
				board->SetAmbulanceLocA(this);
				board->SetAmbulanceLocB(adjacentParkTile);
			}
		}
	}
}

void ATile::setFireEngineLocation(AFireEngine * currentFireEngine)
{
	AEdgeUnit* adjacentParkingWall = nullptr;
	if (ensure(type == ETileType::FireEnginePark))
	{
		if (ensure(adjacentParkTile))
		{
			adjacentParkingWall = isAdjacent(adjacentParkTile);
			if (ensure(adjacentParkingWall))
			{
				if (adjacentParkingWall == frontWall)
				{
					UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());
					if (!leftWall) { setFireEngineLocation(currentFireEngine, EDirection::Left); }
					else { setFireEngineLocation(currentFireEngine, EDirection::Right); }

				}
				else if (adjacentParkingWall == backWall)
				{
					UE_LOG(LogTemp, Warning, TEXT("%s"), *adjacentParkTile->GetName());
					if (!leftWall) { adjacentParkTile->setFireEngineLocation(currentFireEngine, EDirection::Left); }
					else { adjacentParkTile->setFireEngineLocation(currentFireEngine, EDirection::Right); }
				}
				else if (adjacentParkingWall == rightWall)
				{
					UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());
					if (!frontWall) { setFireEngineLocation(currentFireEngine, EDirection::Up); }
					else { setFireEngineLocation(currentFireEngine, EDirection::Down); }
				}
				else if (adjacentParkingWall == leftWall)
				{
					UE_LOG(LogTemp, Warning, TEXT("%s"), *adjacentParkTile->GetName());
					if (!frontWall) { adjacentParkTile->setFireEngineLocation(currentFireEngine, EDirection::Up); }
					else { adjacentParkTile->setFireEngineLocation(currentFireEngine, EDirection::Down); }
				}
			}
		}
	}
}

void ATile::setFireEngineLocation(AFireEngine * currentFireEngine, EDirection direction)
{
	FVector FireEngineSocketLocation;
	FRotator FireEngineSocketRotation;
	switch (direction)
	{
	case EDirection::Down:
		FireEngineSocketLocation = TileMesh->GetSocketLocation(FName("VehicleBot"));
		FireEngineSocketRotation = TileMesh->GetSocketRotation(FName("VehicleBot"));
		break;
	case EDirection::Up:
		FireEngineSocketLocation = TileMesh->GetSocketLocation(FName("VehicleTop"));
		FireEngineSocketRotation = TileMesh->GetSocketRotation(FName("VehicleTop"));
		break;
	case EDirection::Left:
		FireEngineSocketLocation = TileMesh->GetSocketLocation(FName("VehicleLeft"));
		FireEngineSocketRotation = TileMesh->GetSocketRotation(FName("VehicleLeft"));
		break;
	case EDirection::Right:
		FireEngineSocketLocation = TileMesh->GetSocketLocation(FName("VehicleRight"));
		FireEngineSocketRotation = TileMesh->GetSocketRotation(FName("VehicleRight"));
		break;
	default:
		// impossible
		break;
	}
	if (ensure(adjacentParkTile))
	{
		if (ensure(board))
		{
			if (board->GetFireEngine())
			{
				board->GetFireEngine()->SetActorLocation(FireEngineSocketLocation);
				board->GetFireEngine()->SetActorRotation(FireEngineSocketRotation + FRotator(0, 90, 0));
				board->SetFireEngineLocA(this);
				board->SetFireEngineLocB(adjacentParkTile);
			}
		}
	}
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
			if (ensure(localPawn))
			{
				if (localPawn->GetPlacedOn())
				{
					FindPathToCurrent(localPawn);
				}
			}
			break;
		case EGameOperations::ChopWall:
			break;
		case EGameOperations::ExtinguishFire:
			if (!ensure(localPawn)) return;
			// check if the player can extinguish fire on this tile
			if (AdjacentToPawn(localPawn)) {
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
		case EGameOperations::FlipPOI:
			// UE_LOG(LogTemp, Warning, TEXT("Mouse Over"));
			if (!ensure(localPawn)) return;
			if (POIStatus == EPOIStatus::Hided)
			{
				if (localPawn->GetFireFighterRole() == ERoleType::ImagingTechnician)
				{
					PlaneColorSwitch(ableMat);
				}
				else if (localPawn->GetFireFighterRole() == ERoleType::RescueDog)
				{
					if (AdjacentToPawn(localPawn)) { PlaneColorSwitch(ableMat); }
					else { PlaneColorSwitch(unableMat); }
				}
			}
			else { PlaneColorSwitch(unableMat); }
			break;
		case EGameOperations::Dodge:
			if (!ensure(localPawn)) return;
			if (!localPawn->GetCanDodge()) return;
			if (fireStatus != EFireStatus::Fire)
			{
				if (AdjacentToPawn(localPawn))
				{
					if (localPawn->GetCurrentAP() >= localPawn->GetDodgeConsumption())
					{
						PlaneColorSwitch(ableMat);
						return;
					}
				}
			}
			PlaneColorSwitch(unableMat);
			break;
		case EGameOperations::Squeeze:
			if (!ensure(localPawn)) return;
			UE_LOG(LogTemp, Warning, TEXT("Squeeze Over."));
			if (localPawn->GetCarriedVictim() == nullptr)
			{
				if (ensure(localPawn->GetPlacedOn()))
				{
					AEdgeUnit* adjacentEdge = isAdjacent(localPawn->GetPlacedOn());
					if (adjacentEdge != nullptr)
					{
						AWall* adjacentWall = Cast<AWall>(adjacentEdge);
						if (adjacentWall)
						{
							if (adjacentWall->isChoped)
							{
								if (localPawn->GetCurrentAP() >= localPawn->GetSequeezeConsumption())
								{
									PlaneColorSwitch(ableMat);
									break;
								}
							}
						}
					}
				}
			}
			PlaneColorSwitch(unableMat);
			break;
		case EGameOperations::Command:
			if (!localPlayer->GetCommanded()) return;
			FindPathToCurrent(localPlayer->GetCommanded());
			break;
		case EGameOperations::DriveAmbulance:
			if (!ensure(localPawn)) { return; }
			if (this->type == ETileType::AmbulancePark)
			{
				if (ensure(board) && ensure(board->GetAmbulanceLocA()) && ensure(board->GetAmbulance()))
				{
					if ((!board->GetAmbulanceLocA()->GetLeft() && !this->GetRight())
						|| (!board->GetAmbulanceLocA()->GetRight() && !this->GetLeft())
						|| (!board->GetAmbulanceLocA()->GetBack() && !this->GetFront())
						|| (!board->GetAmbulanceLocA()->GetFront() && !this->GetBack()))
					{
						if (localPawn->GetCurrentAP() >= 2 * localPawn->GetDriveConsumption())
						{
							PlaneColorSwitch(ableMat);
						}
						else
						{
							PlaneColorSwitch(unableMat);
						}
					}
					else
					{
						if (localPawn->GetCurrentAP() >= localPawn->GetDriveConsumption())
						{
							PlaneColorSwitch(ableMat);
						}
						else
						{
							PlaneColorSwitch(unableMat);
						}
					}
				}
			}
			else
			{
				PlaneColorSwitch(unableMat);
			}
			break;
		case EGameOperations::Radio:
			if (type == ETileType::AmbulancePark && this != board->GetAmbulanceLocA() && this != board->GetAmbulanceLocB())
			{
				AAmbulance* calledAmbulance = nullptr;
				if (ensure(board))
				{
					calledAmbulance = board->GetAmbulance();
					if (ensure(calledAmbulance))
					{
						if ((!board->GetAmbulanceLocA()->GetLeft() && !this->GetRight())
							|| (!board->GetAmbulanceLocA()->GetRight() && !this->GetLeft())
							|| (!board->GetAmbulanceLocA()->GetBack() && !this->GetFront())
							|| (!board->GetAmbulanceLocA()->GetFront() && !this->GetBack()))
						{
							if (localPawn->GetCurrentAP() >= 2 * localPawn->GetDriveConsumption())
							{
								PlaneColorSwitch(ableMat);
							}
							else
							{
								PlaneColorSwitch(unableMat);
							}
						}
						else
						{
							if (localPawn->GetCurrentAP() >= localPawn->GetDriveConsumption())
							{
								PlaneColorSwitch(ableMat);
							}
							else
							{
								PlaneColorSwitch(unableMat);
							}
						}
					}
				}
			}
			else
			{
				PlaneColorSwitch(unableMat);
			}
			break;
		case EGameOperations::DriveFireEngine:
			if (this->type == ETileType::FireEnginePark)
			{
				if (ensure(board) && ensure(board->GetFireEngineLocA()) && ensure(board->GetFireEngine()))
				{
					if ((!board->GetFireEngineLocA()->GetLeft() && !this->GetRight())
						|| (!board->GetFireEngineLocA()->GetRight() && !this->GetLeft())
						|| (!board->GetFireEngineLocA()->GetBack() && !this->GetFront())
						|| (!board->GetFireEngineLocA()->GetFront() && !this->GetBack()))
					{
						if (localPawn->GetCurrentAP() >= 2 * localPawn->GetDriveConsumption())
						{
							PlaneColorSwitch(ableMat);
						}
						else
						{
							PlaneColorSwitch(unableMat);
						}
					}
					else
					{
						if (localPawn->GetCurrentAP() >= localPawn->GetDriveConsumption())
						{
							PlaneColorSwitch(ableMat);
						}
						else
						{
							PlaneColorSwitch(unableMat);
						}
					}
				}
			}
			else
			{
				PlaneColorSwitch(unableMat);
			}
			break;
		case EGameOperations::GetOutAmbulance:
			if (ensure(board))
			{
				// UE_LOG(LogTemp, Warning, TEXT("this -> %s, LocA = %s"), *GetName(), *board->GetAmbulanceLocA()->GetName());
				if (ensure(board->GetAmbulanceLocA()) && ensure(board->GetAmbulanceLocB()))
				{
					if (this == board->GetAmbulanceLocA() || this == board->GetAmbulanceLocB())
					{
						PlaneColorSwitch(ableMat);
					}
					else
					{
						PlaneColorSwitch(unableMat);
					}
				}
			}
			break;
		case EGameOperations::GetOutFireEngine:
			if (ensure(board))
			{
				if (ensure(board->GetAmbulanceLocA()) && ensure(board->GetAmbulanceLocB()))
				{
					if (this == board->GetFireEngineLocA() || this == board->GetFireEngineLocB())
					{
						PlaneColorSwitch(ableMat);
					}
					else
					{
						PlaneColorSwitch(unableMat);
					}
				}
			}
			break;
		case EGameOperations::PlaceFireEngine:
			if (type == ETileType::FireEnginePark) {
				PlaneColorSwitch(ableMat);
			}
			else {
				PlaneColorSwitch(unableMat);
			}
			break;
		case EGameOperations::PlaceAmbulance:
			if (type == ETileType::AmbulancePark) {
				PlaneColorSwitch(ableMat);
			}
			else {
				PlaneColorSwitch(unableMat);
			}
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

void ATile::ShowSplashEffect()
{
	if (ensure(SplashEffect)) {
		SplashEffect->DeactivateSystem();
		SplashEffect->ActivateSystem();
	}
	splashOccured = !splashOccured;
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
			UE_LOG(LogTemp, Warning, TEXT("Cost to this tile is: %d"), costToHere);
			// Move operations are done below
			if (isReady && canMoveTo) {
				if (!ensure(localPlayer)) return;
				if (!ensure(localPawn)) return;
				if (!localPawn->GetPlacedOn()) return;
				localPlayer->SetNone();
				if (HasAuthority()) {
					PawnMoveToHere(localPawn, pathToHere);
					// check get free AP or not
					if (localPawn->CheckIsVicinty(board->GetVeteranLoc()) && localPawn->GetFireFighterRole() != ERoleType::Veteran)
					{
						if (!localPawn->HasGainedAPThisTurn())
						{
							localPawn->SetHasGainedAPThisTurn(true);
							localPawn->AdjustFireFighterAP(1);
							UE_LOG(LogTemp, Warning, TEXT("Get free AP"));
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Already got free AP"));
						}
					}
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
			if (!AdjacentToPawn(localPawn)) return;
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
		case EGameOperations::FlipPOI:
			if (!ensure(localPawn)) return;
			if (!ensure(localPlayer)) return;
			localPlayer->ServerRevealPOI(this);
			if (localPawn->GetFireFighterRole() == ERoleType::ImagingTechnician)
			{
				localPawn->AdjustFireFighterAP(-1);
			}
			break;
		case EGameOperations::Dodge:
			if (!ensure(localPawn)) return;
			if (!localPawn->GetCanDodge()) return;
			UE_LOG(LogTemp, Warning, TEXT("Dodge Clicked"));
			UE_LOG(LogTemp, Warning, TEXT("AdjacentToPawn(localPawn)::%d "), AdjacentToPawn(localPawn));
			if (fireStatus == EFireStatus::Fire) { return; }
			if (AdjacentToPawn(localPawn))
			{
				if (localPawn->GetCurrentAP() >= localPawn->GetDodgeConsumption())
				{
					pathToHere.Empty();
					pathToHere.Add(this);
					localPlayer->ServerMovePawn(this, localPawn, pathToHere);
					// to make the placing visible to operation client
					localPawn->SetActorLocation(TileMesh->GetSocketLocation("VisualEffects"));
					// do ap adjustment
					localPawn->AdjustFireFighterAP(-localPawn->GetDodgeConsumption());

					localPlayer->SetNone();
				}
			}
			break;
		case EGameOperations::Squeeze:
			UE_LOG(LogTemp, Warning, TEXT("Squeeze Clicked."));
			if (localPawn->GetCarriedVictim()) { return; }
			if (ensure(localPawn->GetPlacedOn()))
			{
				AEdgeUnit* adjacentEdge = isAdjacent(localPawn->GetPlacedOn());
				if (adjacentEdge != nullptr)
				{
					AWall* adjacentWall = Cast<AWall>(adjacentEdge);
					if (adjacentWall)
					{
						if (adjacentWall->isChoped)
						{
							if (localPawn->GetCurrentAP() >= localPawn->GetSequeezeConsumption())
							{
								pathToHere.Empty();
								pathToHere.Add(this);
								localPlayer->ServerMovePawn(this, localPawn, pathToHere);
								// to make the placing visible to operation client
								localPawn->SetActorLocation(TileMesh->GetSocketLocation("VisualEffects"));
								// do ap adjustment
								localPawn->AdjustFireFighterAP(-localPawn->GetSequeezeConsumption());
							}
						}
					}
				}
			}
			break;
		case EGameOperations::Command:
			// check if this tile is ready and can move to
			if (isReady && canMoveTo) {
				UE_LOG(LogTemp, Warning, TEXT("Can move to here"));
				AFireFighterPawn* commanded = localPlayer->GetCommanded();
				if (commanded && ensure(localPawn)) {
					// clear all tiles before doing this 
					if (ensure(board)) {
						board->ClearAllTile();
					}
					// if the player is commanding a firefighter indicate the trace for the commanded firefighter
					TArray<ATile*> traceTiles;
					ATile* start = commanded->GetPlacedOn();
					ATile* goal = this;
					int32 cost = GeneralTypes::AStarShortest(localPawn->GetCommandAP(), start, goal, traceTiles);
					UE_LOG(LogTemp, Warning, TEXT("Path to here for commanded pawn is: %d"), traceTiles.Num());
					// do server notify
					localPlayer->SetNone();
					localPlayer->CancelCommand();
					localPlayer->ServerCommandTileOperation(commanded, localPawn, traceTiles);
				}
			}
			break;
		case EGameOperations::DriveFireEngine:
			UE_LOG(LogTemp, Warning, TEXT("Drive Fire Engine Operation."));
			if (type == ETileType::FireEnginePark && this != board->GetFireEngineLocA() && this != board->GetFireEngineLocB())
			{
				if (ensure(localPawn))
				{
					if (ensure(board) && ensure(board->GetFireEngineLocA()) && ensure(board->GetFireEngine()))
					{
						if ((!board->GetFireEngineLocA()->GetLeft() && !this->GetRight())
							|| (!board->GetFireEngineLocA()->GetRight() && !this->GetLeft())
							|| (!board->GetFireEngineLocA()->GetBack() && !this->GetFront())
							|| (!board->GetFireEngineLocA()->GetFront() && !this->GetBack()))
						{
							if (localPawn->GetCurrentAP() >= 2 * localPawn->GetDriveConsumption())
							{
								localPlayer->ServerMoveFireEngine(board->GetFireEngine(), this);
								localPawn->AdjustFireFighterAP(-2 * localPawn->GetDriveConsumption());
								board->GetFireEngine()->setFireEngineUI(false);
							}
						}
						else
						{
							if (localPawn->GetCurrentAP() >= localPawn->GetDriveConsumption())
							{
								localPlayer->ServerMoveFireEngine(board->GetFireEngine(), this);
								localPawn->AdjustFireFighterAP(-localPawn->GetDriveConsumption());
								board->GetFireEngine()->setFireEngineUI(false);
							}
						}
					}
				}
			}
			break;
		case EGameOperations::DriveAmbulance:
			UE_LOG(LogTemp, Warning, TEXT("Drive Ambulance Operation."));
			if (type == ETileType::AmbulancePark && this != board->GetAmbulanceLocA() && this != board->GetAmbulanceLocB())
			{
				if (ensure(localPawn))
				{
					if (ensure(board) && ensure(board->GetAmbulanceLocA()) && ensure(board->GetAmbulance()))
					{
						if ((!board->GetAmbulanceLocA()->GetLeft() && !this->GetRight())
							|| (!board->GetAmbulanceLocA()->GetRight() && !this->GetLeft())
							|| (!board->GetAmbulanceLocA()->GetBack() && !this->GetFront())
							|| (!board->GetAmbulanceLocA()->GetFront() && !this->GetBack()))
						{
							if (localPawn->GetCurrentAP() >= 2 * localPawn->GetDriveConsumption())
							{
								localPlayer->ServerMoveAmbulance(board->GetAmbulance(), this);
								localPawn->AdjustFireFighterAP(-2 * localPawn->GetDriveConsumption());
								board->GetAmbulance()->setAmbulanceUI(false);
							}
						}
						else
						{
							if (localPawn->GetCurrentAP() >= localPawn->GetDriveConsumption())
							{
								localPlayer->ServerMoveAmbulance(board->GetAmbulance(), this);
								localPawn->AdjustFireFighterAP(-localPawn->GetDriveConsumption());
								board->GetAmbulance()->setAmbulanceUI(false);
							}
						}
					}
				}
			}
			break;
		case EGameOperations::GetOutAmbulance:
			UE_LOG(LogTemp, Warning, TEXT("Get out of ambulance."));
			if (type == ETileType::AmbulancePark)
			{
				if (this == board->GetAmbulanceLocA() || this == board->GetAmbulanceLocB())
				{
					if (!ensure(localPlayer)) return;
					if (!ensure(localPawn)) return;
					if (!ensure(board)) return;
					AAmbulance* ambulanceOnTile = board->GetAmbulance();
					if (ensure(ambulanceOnTile))
					{
						localPlayer->ServerGetOutAmbulance(localPawn, this, ambulanceOnTile);
					}
				}
				localPlayer->SetNone();
			}
			break;
		case EGameOperations::GetOutFireEngine:
			UE_LOG(LogTemp, Warning, TEXT("Get out of ambulance."));
			if (type == ETileType::FireEnginePark)
			{
				if (this == board->GetFireEngineLocA() || this == board->GetFireEngineLocB())
				{
					if (!ensure(localPlayer)) return;
					if (!ensure(localPawn)) return;

					if (!ensure(board)) return;
					AFireEngine* fireEngineOnTile = board->GetFireEngine();
					if (ensure(fireEngineOnTile))
					{
						localPlayer->ServerGetOutFireEngine(localPawn, this, fireEngineOnTile);
					}
				}
				localPlayer->SetNone();
			}
			break;
		case EGameOperations::Radio:
			UE_LOG(LogTemp, Warning, TEXT("Radio/Call the ambulance."));
			if (type == ETileType::AmbulancePark && this != board->GetAmbulanceLocA() && this != board->GetAmbulanceLocB())
			{
				AAmbulance* calledAmbulance = nullptr;
				if (ensure(board))
				{
					calledAmbulance = board->GetAmbulance();
					if (ensure(calledAmbulance))
					{
						if ((!board->GetAmbulanceLocA()->GetLeft() && !this->GetRight())
							|| (!board->GetAmbulanceLocA()->GetRight() && !this->GetLeft())
							|| (!board->GetAmbulanceLocA()->GetBack() && !this->GetFront())
							|| (!board->GetAmbulanceLocA()->GetFront() && !this->GetBack()))
						{
							if (localPawn->GetCurrentAP() >= 2 * localPawn->GetDriveConsumption())
							{
								localPlayer->ServerMoveAmbulance(calledAmbulance, this);
								localPawn->AdjustFireFighterAP(-2 * localPawn->GetDriveConsumption());
								board->GetAmbulance()->setAmbulanceUI(false);
							}
						}
						else
						{
							if (localPawn->GetCurrentAP() >= localPawn->GetDriveConsumption())
							{
								localPlayer->ServerMoveAmbulance(calledAmbulance, this);
								localPawn->AdjustFireFighterAP(-localPawn->GetDriveConsumption());
								board->GetAmbulance()->setAmbulanceUI(false);
							}
						}
					}
				}
				localPlayer->SetNone();
			}
			break;
		case EGameOperations::PlaceAmbulance:
			if (type != ETileType::AmbulancePark) return;
			// palce the ambulance to this tile, only server does this
			if (HasAuthority()) {
				if (!ensure(board)) return;
				AAmbulance* tempAmb = board->GetAmbulance();
				if (!ensure(tempAmb)) return;
				setAmbulanceLocation(tempAmb);
				localPlayer->SetCurrentOperation(EGameOperations::None);
				tempAmb->ShowAmbulancePlaced(true);
				// as this operation is done, turn can begin
				board->TurnSwitch();
			}
			break;
		case EGameOperations::PlaceFireEngine:
			if (type != ETileType::FireEnginePark) return;
			// place the engine to this tile, only server does this
			if (HasAuthority()) {
				if (!ensure(board)) return;
				AFireEngine* tempEngine = board->GetFireEngine();
				if (!ensure(tempEngine)) return;
				setFireEngineLocation(tempEngine);
				localPlayer->SetCurrentOperation(EGameOperations::PlaceAmbulance);
				localPlayer->PromtPlacingVehicle("Place Ambulance");
				tempEngine->ShowEnginePlaced(true);
			}
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
	// if the tile is command target, do not clear
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

void ATile::FindPathToCurrent(AFireFighterPawn* inPawn)
{
	if (!ensure(inPawn))	return;
	// Do an a* search to find the shortest path to this target location
	TArray<ATile*> traceTiles;
	ATile* start = inPawn->GetPlacedOn();
	ATile* goal = this;
	int32 cost = GeneralTypes::AStarShortest(inPawn->GetCurrentAP(), start, goal, traceTiles);
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
		cost = cost * inPawn->GetMoveConsumption();
		if (inPawn->GetCarriedVictim() || inPawn->GetHazmat()) {
			// if carrying any victim, double the cost
			cost = cost * 2;
			// if the role is rescue dog, double the cost again
			if (inPawn->GetFireFighterRole() == ERoleType::RescueDog)
			{
				cost = cost * 2;
			}
		}
		// here is the case if the firefighter is carring some victim but the trace has fire
		if (hasFire && inPawn->GetCarriedVictim()) {
			for (int32 i = traceTiles.Num() - 1; i >= 0; i--) {
				traceTiles[i]->PlaneColorSwitch(unableMat);
			}
		}
		else if (inPawn->GetIsCommanded()) {
			// here we are chekcing if the captain has enough command ap to command the operation
			if (ensure(localPawn)) {
				if (localPawn->GetCommandAP() >= cost) {
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
			}
		}
		// here the goal is successfully found and could be moved to
		else if (cost <= inPawn->GetCurrentAP() + inPawn->GetMovementAP()) {
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
}

bool ATile::AdjacentToPawn(AFireFighterPawn * inPawn)
{
	if (ensure(inPawn)) {
		// check if the pawn is on the tile
		if (inPawn->GetPlacedOn() == this) {
			return true;
		}
		// check if the pawn is around the tile
		ATile* tempTile = nullptr;
		// check on front tile
		if (frontWall && !frontWall->IsBlocked()) {
			tempTile = frontWall->GetOtherNeighbour(this);
			if (ensure(tempTile)) {
				if (tempTile->placedFireFighters.Contains(inPawn)) {
					return true;
				}
			}
		}
		// check on back tile
		if (backWall && !backWall->IsBlocked()) {
			tempTile = backWall->GetOtherNeighbour(this);
			if (ensure(tempTile)) {
				if (tempTile->placedFireFighters.Contains(inPawn)) {
					return true;
				}
			}
		}
		// check on left tile
		if (leftWall && !leftWall->IsBlocked()) {
			tempTile = leftWall->GetOtherNeighbour(this);
			if (ensure(tempTile)) {
				if (tempTile->placedFireFighters.Contains(inPawn)) {
					return true;
				}
			}
		}
		// check on right tile
		if (rightWall && !rightWall->IsBlocked()) {
			tempTile = rightWall->GetOtherNeighbour(this);
			if (ensure(tempTile)) {
				if (tempTile->placedFireFighters.Contains(inPawn)) {
					return true;
				}
			}
		}
	}
	return false;
}

AEdgeUnit* ATile::isAdjacent(ATile * targetTile)
{
	if (ensure(targetTile))
	{
		if (frontWall)
		{
			if (frontWall->GetOtherNeighbour(this) == targetTile) { return frontWall; }
		}
		if (rightWall)
		{
			if (rightWall->GetOtherNeighbour(this) == targetTile) { return rightWall; }
		}
		if (leftWall)
		{
			if (leftWall->GetOtherNeighbour(this) == targetTile) { return leftWall; }
		}
		if (backWall)
		{
			if (backWall->GetOtherNeighbour(this) == targetTile) { return backWall; }
		}
	}
	return nullptr;
}

FTileSaveInfo ATile::SaveTile()
{
	FTileSaveInfo tileInfo;
	tileInfo.fireStatus = fireStatus;
	if (HazmatOnTile) {
		tileInfo.hasHazmat = true;
	}
	if (POIOnTile) {
		tileInfo.hasPOI = true;
	}
	if (isHotSpot) {
		tileInfo.hasHotspot = true;
	}
	tileInfo.victimsOnTile = victims.Num();
	if (placedFireFighters.Num() > 0) {
		for (AFireFighterPawn* firefighter : placedFireFighters) {
			if (firefighter->GetCarriedVictim()) {
				tileInfo.victimsOnTile = tileInfo.victimsOnTile + 1;
			}
			if (firefighter->GetLeading()) {
				tileInfo.victimsOnTile = tileInfo.victimsOnTile + 1;
			}
			if (firefighter->GetHazmat()) {
				tileInfo.hasHazmat = true;
			}
		}
	}
	return tileInfo;
}

void ATile::LoadTile(FTileSaveInfo tileInfo)
{
	// load fire status
	fireStatus = tileInfo.fireStatus;
	if (!ensure(FireEffect) || !ensure(SmokeEffect) || !ensure(BlastEffect))	return;
	switch (tileInfo.fireStatus) {
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
	// load POI
	if (tileInfo.hasPOI) {
		AdvancePOI();
	}
	// load victims
	if (tileInfo.victimsOnTile > 0) {
		for (int32 i = 0; i < tileInfo.victimsOnTile; i++) {
			PlaceVictim();
			board->currentPOI = board->currentPOI + 1;
		}
	}
	// load hot spot on the tile
	if (tileInfo.hasHotspot) {
		isHotSpot = true;
		if (ensure(HotSpotEffect)) {
			HotSpotEffect->ActivateSystem();
		}
	}
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

int32 ATile::GetID()
{
	return xLoc * 8 + yLoc;
}

void ATile::Rep_BlastEffect()
{
	if (!ensure(BlastEffect)) return;
	BlastEffect->DeactivateSystem();
	BlastEffect->ActivateSystem();
}

void ATile::Rep_HotSpotEffect()
{
	if (!ensure(HotSpotEffect))	return;
	if (isHotSpot) { HotSpotEffect->Activate(); }
	else { HotSpotEffect->Deactivate(); }
}

void ATile::Rep_SplashEffect()
{
	if (!ensure(SplashEffect)) return;
	SplashEffect->DeactivateSystem();
	SplashEffect->ActivateSystem();
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
	DOREPLIFETIME(ATile, HazmatOnTile);
	DOREPLIFETIME(ATile, isHotSpot);
	DOREPLIFETIME(ATile, type);
	DOREPLIFETIME(ATile, adjacentParkTile);
	DOREPLIFETIME(ATile, splashOccured);
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