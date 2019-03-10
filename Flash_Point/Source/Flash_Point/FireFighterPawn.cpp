// Fill out your copyright notice in the Description page of Project Settings.

#include "FireFighterPawn.h"
#include "Victim.h"
#include "GameBoard.h"
#include "FPPlayerController.h"


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

int32 AFireFighterPawn::GetFireFighterID()
{
	return fireFighterID;
}

void AFireFighterPawn::SetFireFighterID(int32 inID)
{
	fireFighterID = inID;
}

void AFireFighterPawn::Rep_PawnID()
{
	// when id got, set it the the player
	if (ensure(myOwner) && fireFighterID >= 0) {
		myOwner->SetTurnNum(fireFighterID);
	}
}

void AFireFighterPawn::KnockDown()
{
	UE_LOG(LogTemp, Warning, TEXT("Knock Down"));
}

void AFireFighterPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFireFighterPawn, placedOn);
	DOREPLIFETIME(AFireFighterPawn, currentAP);
	DOREPLIFETIME(AFireFighterPawn, maxAP);
	DOREPLIFETIME(AFireFighterPawn, moveConsumption);
	DOREPLIFETIME(AFireFighterPawn, openConsumption);
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


