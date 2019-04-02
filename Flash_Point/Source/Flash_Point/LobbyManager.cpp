// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyManager.h"
#include "CrewManager.h"


// Sets default values
ALobbyManager::ALobbyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ALobbyManager::AutoJoinLobby(AFireFighterPawn * inPawn, FString playerName)
{
	// this is called when the player joins the curren lobby which automatically assigns a valid role to the firefighter pawn

}

ACrewManager * ALobbyManager::GetCrewManager()
{
	return crewMan;
}

void ALobbyManager::SetCrewManager(ACrewManager * inCrewMan)
{
	crewMan = inCrewMan;
}

void ALobbyManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyManager, joinedPlayer);
	DOREPLIFETIME(ALobbyManager, joinedPawns);
}

// Called when the game starts or when spawned
void ALobbyManager::BeginPlay()
{
	if (HasAuthority()) {
		SetReplicates(true);
	}
	Super::BeginPlay();
	
}

// Called every frame
void ALobbyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

