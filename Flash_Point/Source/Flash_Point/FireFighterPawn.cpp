// Fill out your copyright notice in the Description page of Project Settings.

#include "FireFighterPawn.h"
#include "Victim.h"
#include "GameBoard.h"
#include "FPPlayerController.h"
#include "Tile.h"


// Sets default values
AFireFighterPawn::AFireFighterPawn()
{
	// Create the skeletal mesh as default component
	FireFighter = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Fire Fighter"));
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;

}

ATile * AFireFighterPawn::GetPlacedOn()
{
	return placedOn;
}

void AFireFighterPawn::SetPlacedOn(ATile * tile)
{
	placedOn = tile;
}

int32 AFireFighterPawn::GetCurrentAP()
{
	return currentAP;
}

bool AFireFighterPawn::IsWithEngine()
{
	if (ensure(placedOn)) {
		if (ensure(placedOn->GetGameBoard())) {
			return placedOn->GetGameBoard()->IsEngineTile(placedOn);
		}
	}
	return false;
}

bool AFireFighterPawn::IsWithAmbulance()
{
	if (ensure(placedOn)) {
		if (ensure(placedOn->GetGameBoard())) {
			return placedOn->GetGameBoard()->IsAmbulanceTile(placedOn);
		}
	}
	return false;
}

void AFireFighterPawn::SetCurrentAP(int32 current)
{
	currentAP = current;
	if (HasAuthority()) {
		// only when clients using this to adjust Ap this should be used
		if (ensure(statusBar)) {
			statusBar->AdjustAPBar(currentAP, maxAP);
		}
	}
}

int32 AFireFighterPawn::GetMaxAP()
{
	return maxAP;
}

int32 AFireFighterPawn::GetRestoreAP()
{
	return restoreAP;
}

void AFireFighterPawn::AdjustFireFighterAP(int32 adjustAP)
{
	if (!ensure(myOwner)) return;
	if (currentAP + adjustAP < 0) return;	// negative ap is impossible
	// This is cheating for test purpose
	if (!myOwner->ConsumptionOn() && adjustAP <= 0) return;
	if (HasAuthority()) {
		currentAP += adjustAP;
		// adjust display of ap on statusbar
		if (ensure(statusBar)) {
			statusBar->AdjustAPBar(currentAP, maxAP);
		}
	}
	else {
		myOwner->ServerAdjustAP(this, adjustAP);
	}
}

void AFireFighterPawn::AdjustCAFSFireFighterExtinguishAP(int32 adjustAP)
{
	if (!ensure(myOwner)) return;
	if (currentAP + adjustAP + extinguishAP < 0) return;	// negative ap is impossible
	// This is cheating for test purpose
	if (!myOwner->ConsumptionOn() && adjustAP <= 0) return;
	if (HasAuthority()) {
		if (extinguishAP < -adjustAP)
		{
			adjustAP += extinguishAP;
			extinguishAP = 0;
			currentAP += (adjustAP);
		}
		else 
		{
			extinguishAP += adjustAP;
		}
		// adjust display of ap on statusbar
		if (ensure(statusBar)) {
			statusBar->AdjustAPBar(currentAP, maxAP);
		}
	}
	else {
		myOwner->ServerAdjustCAFSAP(this, adjustAP);
	}
}

void AFireFighterPawn::AdjustSpecialistMoveAP(int32 adjustAP)
{
	if (!ensure(myOwner)) return;
	if (currentAP + adjustAP < 0) return;	// negative ap is impossible
	// This is cheating for test purpose
	if (!myOwner->ConsumptionOn() && adjustAP <= 0) return;
	if (HasAuthority()) {
		if (movementAP < -adjustAP)
		{
			movementAP = 0;
			currentAP += (adjustAP + movementAP);
		}
		else
		{
			movementAP += adjustAP;
		}
		currentAP += adjustAP;
		// adjust display of ap on statusbar
		if (ensure(statusBar)) {
			statusBar->AdjustAPBar(currentAP, maxAP);
		}
	}
	else {
		myOwner->ServerAdjustAP(this, adjustAP);
	}
}

FString AFireFighterPawn::GetFireFighterName()
{
	return fireFighterName;
}

void AFireFighterPawn::SetFireFighterName(FString inName)
{
	fireFighterName = inName;
}

ERoleType AFireFighterPawn::GetFireFighterRole()
{
	return fireFighterRole;
}

void AFireFighterPawn::SetFireFighterRole(ERoleType inRole)
{
	fireFighterRole = inRole;
	// synchronize the view of status bar only on the server side
	if (HasAuthority()) {
		if (ensure(statusBar)) {
			statusBar->ChangeRolePic(inRole);
		}
	}
}

void AFireFighterPawn::AdjustRoleProperties(ERoleType inRole)
{
	ResetProperties();
	// adjust properties with respect to firefighter role type
	switch (inRole)
	{
	case ERoleType::Basic:
		break;
	case ERoleType::Paramedic:
		extinguishConsumption = 2;
		break;
	case ERoleType::FireCaptain:
		commandAP = 2;
		break;
	case ERoleType::ImagingTechnician:
		break;
	case ERoleType::CAFSFirefighter:
		extinguishAP = 3;
		restoreAP = 3;
		break;
	case ERoleType::HazmatTechnician:
		break;
	case ERoleType::Generalist:
		restoreAP = 5;
		break;
	case ERoleType::RescueSpecialist:
		chopConsumption = 1;
		movementAP = 3;
		extinguishConsumption = 2;
		break;
	case ERoleType::Driver:
		deckGunConsumption = 2;
		break;
	case ERoleType::Veteran:
		dodgeConsumption = 1;
		break;
	case ERoleType::RescueDog:
		restoreAP = 12;
		maxAP = 18;
		break;
	default:
		break;
	}
}

void AFireFighterPawn::ResetProperties()
{
	// reset the firefighter's properties to basic
	maxAP = 8;
	if (currentAP > maxAP) {
		currentAP = maxAP;
	}
	extinguishAP = 0;
	movementAP = 0;
	commandAP = 0;
	restoreAP = 4;
	moveConsumption = 1;
	openConsumption = 1;
	chopConsumption = 2;
	extinguishConsumption = 1;
	deckGunConsumption = 4;
}

int32 AFireFighterPawn::GetMoveConsumption()
{
	return moveConsumption;
}

void AFireFighterPawn::SetMoveConsumption(int32 current)
{
	moveConsumption = current;
}

int32 AFireFighterPawn::GetOpenConsumption()
{
	return openConsumption;
}

void AFireFighterPawn::SetOpenConsumption(int32 current)
{
	openConsumption = current;
}

int32 AFireFighterPawn::GetChopConsumption()
{
	return chopConsumption;
}

void AFireFighterPawn::SetChopConsumption(int32 current)
{
	chopConsumption = current;
}

int32 AFireFighterPawn::GetExtinguishConsumption()
{
	return extinguishConsumption;
}

void AFireFighterPawn::SetExtinguishConsumption(int32 current)
{
	extinguishConsumption = current;
}

int32 AFireFighterPawn::GetCrewChangeConsumption()
{
	return crewChangeConsumption;
}

bool AFireFighterPawn::CheckCanExtinguish(int32 baseCost)
{
	int32 actualCost = baseCost * extinguishConsumption;
	// TODO for later fire fighter with row, do extra checks
	if (actualCost <= currentAP + extinguishAP)	return true;
	return false;
}

bool AFireFighterPawn::IsAdjacentToWall(AEdgeUnit * inEdge)
{
	ATile* tempTile = GetPlacedOn();
	if (ensure(tempTile)) {
		if (tempTile->GetFront() == inEdge) { return true; }
		if (tempTile->GetBack() == inEdge) { return true; }
		if (tempTile->GetLeft() == inEdge) { return true; }
		if (tempTile->GetRight() == inEdge) { return true; }
	}
	return false;
}

int32 AFireFighterPawn::GetFireFighterID()
{
	return fireFighterID;
}

void AFireFighterPawn::SetFireFighterID(int32 inID)
{
	if (HasAuthority()) {
		FireFighterSwitchColor(inID);
	}
	fireFighterID = inID;
	if (HasAuthority()) {
		// since we are the server here, relate the newly set firefighter to the UI
		RelateAllFireFighterStatus();
	}
}

void AFireFighterPawn::Rep_PawnID()
{
	// when id got, set it the the player
	if (fireFighterID < 0) return;
	FireFighterSwitchColor(fireFighterID);
	if (ensure(myOwner)) {
		myOwner->SetTurnNum(fireFighterID);
	}
	RelateAllFireFighterStatus();
}

void AFireFighterPawn::RelateAllFireFighterStatus()
{
	// find all fire fighter pawn and bind all their widgets
	// ensure there is a world
	UWorld* world = GetWorld();
	if (ensure(world)) {
		TArray<AActor*> allFireFighters;
		UGameplayStatics::GetAllActorsOfClass(world, AFireFighterPawn::StaticClass(), allFireFighters);
		// only assign correct game board if there is one found
		if (allFireFighters.Num() > 0) {
			// bind all the firefighters to their widgets
			for (auto tempPawn : allFireFighters) {
				AFireFighterPawn* tempFireFighter = Cast<AFireFighterPawn>(tempPawn);
				if (ensure(tempFireFighter)) {
					tempFireFighter->BindStatusWidget();
				}
			}
		}
	}
}

void AFireFighterPawn::BindStatusWidget()
{
	// get the firefighter status in order to visualize the synchronized status
	AFPPlayerController* localController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (ensure(localController)) {
		UE_LOG(LogTemp, Warning, TEXT("Found local player controller on firefighter #%d"), fireFighterID);
		statusBar = localController->GetFireFighterStatusBar(fireFighterID);
		if (ensure(statusBar)) {
			// Initialize all the fire fighter's initial information to the UI
			statusBar->ChangeRolePic(fireFighterRole);
			statusBar->AdjustAPBar(currentAP, maxAP);
			statusBar->ShowPlayerName(fireFighterName);
		}
	}
}

void AFireFighterPawn::Rep_RoleType()
{
	// change the firefighter's role pic on status bar
	if (ensure(statusBar)) {
		statusBar->ChangeRolePic(fireFighterRole);
	}
}

void AFireFighterPawn::Rep_APChanges()
{
	// adjust display of ap on statusbar
	if (ensure(statusBar)) {
		statusBar->AdjustAPBar(currentAP, maxAP);
	}
}

void AFireFighterPawn::Rep_CarryingVictim()
{
	// if the firefighter pawn is not owned by local player, return
	if (myOwner) {
		if (victim) {
			myOwner->NotifyCarryVictim(true);
		}
		else {
			myOwner->NotifyCarryVictim(false);
		}
	}
}

void AFireFighterPawn::Rep_LeadingVictim()
{
}

void AFireFighterPawn::Rep_FireFighterknockDownRelocate()
{
	if (ensure(placedOn)) {
		// relocate the firefighter to the tile
		SetActorLocation(placedOn->GetTileMesh()->GetSocketLocation(FName("VisualEffects")));
	}
}

void AFireFighterPawn::Rep_FireFighterDodge()
{
	AFPPlayerController* fireFighterController = Cast<AFPPlayerController>(GetController());
	if (ensure(fireFighterController))
	{
		fireFighterController->NotifyPlayerDodge();
	}
}

void AFireFighterPawn::OnOverFirefighter(AActor * TouchedActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Firefighter Over"));
	// if the current firefighter is commanded by the captain, do not change color
	if (isCommanded) return;
	// only do action if the local player's operation is command
	UWorld* world = GetWorld();
	if (ensure(world)) {
		AFPPlayerController* localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
		if (!ensure(localPlayer)) return;
		// a player cannot comman own pawn
		if (localPlayer->GetPawn() == this) return;
		UE_LOG(LogTemp, Warning, TEXT("Firefighter Over"));
		if (!(localPlayer->GetCurrentOperation() == EGameOperations::Command)) return;
		ShowControllable(false, true);
	}
}

void AFireFighterPawn::OnClickedFirefighter(AActor * TouchedActor, FKey ButtonPressed)
{
	UWorld* world = GetWorld();
	if (ensure(world)) {
		AFPPlayerController* localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
		if (!ensure(localPlayer)) return;
		// a player cannot comman own pawn
		if (localPlayer->GetPawn() == this) return;
		if (!(localPlayer->GetCurrentOperation() == EGameOperations::Command)) return;
		// When the firefighetr is clicekd by a player with command operation, set the firefighter to commanded
		localPlayer->EnableOperations(false);
		localPlayer->SetCommanded(this);
		isCommanded = true;
		ShowIsCommanded(true);
	}
}

void AFireFighterPawn::OnLeftFirefighter(AActor * TouchedActor)
{
	// if the current firefighter is commanded by the captain, do not change color
	if (isCommanded) return;
	UWorld* world = GetWorld();
	if (ensure(world)) {
		AFPPlayerController* localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
		if (!ensure(localPlayer)) return;
		// a player cannot comman own pawn
		if (localPlayer->GetPawn() == this) return;
		// disable the showing
		ShowControllable(true, false);
	}
}

void AFireFighterPawn::KnockDown()
{
	UE_LOG(LogTemp, Warning, TEXT("Knock Down"));
	
	ATile* tile = GetPlacedOn();
	
	if (tile) 
	{
		// current pawn location
		int32 x, y;
		if (tile->GetLocation(x, y))
		{
			UE_LOG(LogTemp, Warning, TEXT("%d, %d"), x, y);
			// get gameboard from local player controller
			AGameBoard* tempBoard = nullptr;
			AFPPlayerController* tempPlayer = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
			if (ensure(tempPlayer)) {
				tempBoard = tempPlayer->GetGameBoard();
			}
			if (tempBoard)
			{
				int32 shortestPath = 80;
				ATile* targetTile = nullptr;
				for (ATile* ambulanceTile : tempBoard->GetAmbulanceTiles())
				{
					if (ambulanceTile)
					{
						int32 ambulanceTile_x, ambulanceTile_y;
						if (ambulanceTile->GetLocation(ambulanceTile_x, ambulanceTile_y))
						{
							int32 pathCost = FMath::Abs(ambulanceTile_x - x) + FMath::Abs(ambulanceTile_y - y);
							if (pathCost < shortestPath)
							{
								shortestPath = pathCost;
								targetTile = ambulanceTile;
							}
						}
					}
				}
				if (targetTile)
				{
					placedOn->GetPlacedFireFighters()->Remove(this);
					SetActorLocation(targetTile->GetTileMesh()->GetSocketLocation(FName("VisualEffects")));
					placedOn = targetTile;
					placedOn->GetPlacedFireFighters()->Add(this);
					relocationFlag = !relocationFlag;
				}

				if (victim)
				{
					tempBoard->SetVictimLostNum(tempBoard->victimLostNum + 1);
					victim->Destroy();
					victim = nullptr;
					tempBoard->SetCurrentPOI(tempBoard->currentPOI - 1);
				}
			}
		}
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("wrong"));
	}
	
}

//Set visibility
void AFireFighterPawn::SetVisibility(bool status){
	FireFighter->SetVisibility(status);
}

void AFireFighterPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFireFighterPawn, placedOn);
	DOREPLIFETIME(AFireFighterPawn, restoreAP);
	DOREPLIFETIME(AFireFighterPawn, currentAP);
	DOREPLIFETIME(AFireFighterPawn, maxAP);
	DOREPLIFETIME(AFireFighterPawn, moveConsumption);
	DOREPLIFETIME(AFireFighterPawn, openConsumption);
	DOREPLIFETIME(AFireFighterPawn, chopConsumption);
	DOREPLIFETIME(AFireFighterPawn, extinguishConsumption);
	DOREPLIFETIME(AFireFighterPawn, victim);
	DOREPLIFETIME(AFireFighterPawn, fireFighterID);
	DOREPLIFETIME(AFireFighterPawn, fireFighterRole);
	DOREPLIFETIME(AFireFighterPawn, relocationFlag);
	DOREPLIFETIME(AFireFighterPawn, fireFighterName);
	DOREPLIFETIME(AFireFighterPawn, deckGunConsumption);
	DOREPLIFETIME(AFireFighterPawn, crewChangeConsumption);
	DOREPLIFETIME(AFireFighterPawn, dodgeConsumption);
	DOREPLIFETIME(AFireFighterPawn, serverDodgeFlag);
	DOREPLIFETIME(AFireFighterPawn, extinguishAP);
	DOREPLIFETIME(AFireFighterPawn, movementAP);
	DOREPLIFETIME(AFireFighterPawn, commandAP);
}

bool AFireFighterPawn::GetCanDodge()
{
	if (hasDodgeOperation)
	{
		if (currentAP >= dodgeConsumption)
		{
			ATile* currentTile = placedOn;
			if (ensure(currentTile))
			{
				AEdgeUnit* tempEdge;
				tempEdge = currentTile->GetLeft();

				if (tempEdge)
				{
					if (canDodgeAcross(tempEdge))
					{
						return true;
					}
				}

				tempEdge = currentTile->GetRight();
				if (tempEdge)
				{
					if (canDodgeAcross(tempEdge))
					{
						return true;
					}
				}

				tempEdge = currentTile->GetFront();
				if (tempEdge)
				{
					if (canDodgeAcross(tempEdge))
					{
						return true;
					}
				}

				tempEdge = currentTile->GetBack();
				if (tempEdge)
				{
					if (canDodgeAcross(tempEdge))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool AFireFighterPawn::canDodgeAcross(AEdgeUnit* targetEdge)
{
	if (ensure(targetEdge))
	{
		if (!targetEdge->IsBlocked())
		{
			ATile* targetTile = targetEdge->GetOtherNeighbour(placedOn);
			if (ensure(targetTile))
			{
				if (targetTile->GetFireStatus() != EFireStatus::Fire)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void AFireFighterPawn::InitializeFireFighter()
{
	// find the owner of the firefighter
	if (GetController()) {
		UE_LOG(LogTemp, Warning, TEXT("Possessed"));
	}
	AFPPlayerController* owningPlayer = Cast<AFPPlayerController>(GetController());
	if (ensure(owningPlayer)) {
		UE_LOG(LogTemp, Warning, TEXT("Got Player controller"));
		myOwner = owningPlayer;
		playingBoard = myOwner->GetGameBoard();
		// Get the name of this pawn and set it
		owningPlayer->ServerSetFireFighterName(this, owningPlayer->GetPlayerName());
	}

	// if the owning player is the local player controller, display the decal to show controllability
	UWorld* world = GetWorld();
	if (ensure(world)) {
		if (owningPlayer == world->GetFirstPlayerController()) {
			ShowControllable(true, true);
		}
	}

	// get the firefighter ID of the firefighter
	if (ensure(playingBoard)) {
		UE_LOG(LogTemp, Warning, TEXT("Got board"));
		if (HasAuthority()) {
			fireFighterID = playingBoard->JoinBoard();
			owningPlayer->SetTurnNum(fireFighterID);
			RelateAllFireFighterStatus();
		}
		else {
			if (ensure(owningPlayer)) {
				owningPlayer->ServerGetFireFighterID(this, playingBoard);
			}
		}
	}
}

// Called when the game starts or when spawned
void AFireFighterPawn::BeginPlay()
{
	Super::BeginPlay();
	InitializeFireFighter();
	// bind mouse over click left functions
	OnBeginCursorOver.AddDynamic(this, &AFireFighterPawn::OnOverFirefighter);
	OnClicked.AddDynamic(this, &AFireFighterPawn::OnClickedFirefighter);
	OnEndCursorOver.AddDynamic(this, &AFireFighterPawn::OnLeftFirefighter);
}

// Called every frame
void AFireFighterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFireFighterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

AVictim* AFireFighterPawn::GetVictim()
{
	return victim;
}

void AFireFighterPawn::SetVictim(AVictim * victim)
{
	this->victim = victim;
}

AVictim * AFireFighterPawn::GetLeading()
{
	return leadVictim;
}

void AFireFighterPawn::SetLeading(AVictim * victim)
{
	leadVictim = victim;
}

AHazmat * AFireFighterPawn::GetHazmat()
{
	return hazmat;
}

void AFireFighterPawn::SetHazmat(AHazmat * hazmat)
{
	this->hazmat = hazmat;
}


