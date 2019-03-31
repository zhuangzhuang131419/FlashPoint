// Fill out your copyright notice in the Description page of Project Settings.

#include "Ambulance.h"
#include "GameBoard.h"


// Sets default values
AAmbulance::AAmbulance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAmbulance::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmbulance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmbulance::SetAmbulancePosition(int32 newPos)
{
	currentPosition = newPos;
}

int32 AAmbulance::GetAmbulancePosition()
{
	return currentPosition;
}

void AAmbulance::RescueVictims(TArray<AVictim*>* victims, ATile* currentTile)
{
	for (int i = 0; i < victims->Num(); i++)
	{
		AVictim* tempVictim = (*victims)[i];
		if (!tempVictim) continue;
		tempVictim->Destroy();
		currentTile->SetPOIStatus(EPOIStatus::Empty);
		currentTile->GetGameBoard()->SetVictimSavedNum(
			currentTile->GetGameBoard()->victimSavedNum + 1
		);
		currentTile->GetGameBoard()->SetCurrentPOI(
			currentTile->GetGameBoard()->currentPOI - 1
		);
	}
}