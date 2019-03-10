// Fill out your copyright notice in the Description page of Project Settings.

#include "FireFighterPawn.h"
#include "Victim.h"
#include "GameBoard.h"
#include "FPPlayerController.h"
#include "Tile.h"


// Sets default values
AFireFighterPawn::AFireFighterPawn()
{
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

void AFireFighterPawn::SetCurrentAP(int32 current)
{
	currentAP = current;
}

void AFireFighterPawn::AdjustFireFighterAP(int32 adjustAP)
{
	if (!ensure(myOwner)) return;
	if (currentAP + adjustAP < 0) return;	// negative ap is impossible
	// This is cheating for test purpose
	if (!myOwner->ConsumptionOn() && adjustAP <= 0) return;
	if (HasAuthority()) {
		currentAP += adjustAP;
	}
	else {
		myOwner->ServerAdjustAP(this, adjustAP);
	}
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

bool AFireFighterPawn::CheckCanExtinguish(int32 baseCost)
{
	int32 actualCost = baseCost * extinguishConsumption;
	// TODO for later fire fighter with row, do extra checks
	if (actualCost < currentAP)	return true;
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
}

void AFireFighterPawn::Rep_PawnID()
{
	// when id got, set it the the player
	if (fireFighterID < 0) return;
	FireFighterSwitchColor(fireFighterID);
	if (ensure(myOwner)) {
		myOwner->SetTurnNum(fireFighterID);
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
			// gameboard
			if (playingBoard)
			{
				int32 shortestPath = 80;
				ATile* targetTile = nullptr;
				for (ATile* ambulanceTile : playingBoard->GetAmbulanceTiles())
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
				}
			}
		}
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("wrong"));
	}
	
}

void AFireFighterPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFireFighterPawn, placedOn);
	DOREPLIFETIME(AFireFighterPawn, currentAP);
	DOREPLIFETIME(AFireFighterPawn, maxAP);
	DOREPLIFETIME(AFireFighterPawn, moveConsumption);
	DOREPLIFETIME(AFireFighterPawn, openConsumption);
	DOREPLIFETIME(AFireFighterPawn, chopConsumption);
	DOREPLIFETIME(AFireFighterPawn, extinguishConsumption);
	DOREPLIFETIME(AFireFighterPawn, victim);
	DOREPLIFETIME(AFireFighterPawn, fireFighterID);
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
	}

	// get the firefighter ID of the firefighter
	if (ensure(playingBoard)) {
		UE_LOG(LogTemp, Warning, TEXT("Got board"));
		if (HasAuthority()) {
			fireFighterID = playingBoard->JoinBoard();
			owningPlayer->SetTurnNum(fireFighterID);
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

bool AFireFighterPawn::CarrayVictim(AVictim* Citizen)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, TEXT("Added"));
	if (victim == nullptr)
	{
		victim = Citizen;
		return true;
	}

	return false;
}

AVictim* AFireFighterPawn::GetVictim()
{
	return victim;
}

void AFireFighterPawn::SetVictim(AVictim * victim)
{
	this->victim = victim;
}


