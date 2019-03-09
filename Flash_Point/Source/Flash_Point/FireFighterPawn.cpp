// Fill out your copyright notice in the Description page of Project Settings.

#include "FireFighterPawn.h"
#include "Victim.h"


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
}

// Called when the game starts or when spawned
void AFireFighterPawn::BeginPlay()
{
	Super::BeginPlay();
	
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


