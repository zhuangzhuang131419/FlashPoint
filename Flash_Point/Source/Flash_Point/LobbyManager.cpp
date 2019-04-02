// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyManager.h"
#include "CrewManager.h"
#include "FireFighterPawn.h"
#include "FPPlayerController.h"
#include "MenuSystem/LobbyUI.h"


// Sets default values
ALobbyManager::ALobbyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ALobbyManager::AutoJoinLobby(AFireFighterPawn * inPawn, FString playerName)
{
	// this is called when the player joins the curren lobby which automatically assigns a valid role to the firefighter pawn
	if (!ensure(inPawn)) return;
	if (!ensure(crewMan)) return;
	// auto select a role for the firefighter first
	crewMan->AutoSelectRole(inPawn);
	inPawn->SetFireFighterLobbyID(joinedPlayer);
	joinedPawns.Add(inPawn);
	// as the firefighetr's lobby role updates, if in server, update the UI
	if (HasAuthority()) {
		UWorld* world = GetWorld();
		if (ensure(world)) {
			AFPPlayerController* localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
			if (ensure(localPlayer)) {
				AFireFighterPawn* localPawn = Cast<AFireFighterPawn>(localPlayer->GetPawn());
				if (localPawn == inPawn) {
					UE_LOG(LogTemp, Warning, TEXT("Local pawn joining"));
					// here we know this is local pawn and we are server, so update status manually
					if (ensure(lobbyUI) && localPawn) {
						lobbyUI->ShowSelectedRole(localPawn->GetFireFighterLobbyRole());
					}
				}
			}
		}
	}
}

ACrewManager * ALobbyManager::GetCrewManager()
{
	return crewMan;
}

void ALobbyManager::SetCrewManager(ACrewManager * inCrewMan)
{
	crewMan = inCrewMan;
}

void ALobbyManager::BindLobbyUI(ULobbyUI * inLobbyUI)
{
	lobbyUI = inLobbyUI;
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

