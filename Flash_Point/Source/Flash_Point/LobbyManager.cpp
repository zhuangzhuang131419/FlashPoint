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
	joinedPlayer++;
	// update all corresponding status for the firefighter, where the ready status is set to waiting
	UpdatePlayerStatus(inPawn->GetFireFighterLobbyID(), playerName, inPawn->GetFireFighterLobbyRole(), EJoinStatus::Waiting);
	// as the firefighetr's lobby role updates, if in server, update the UI
	if (HasAuthority()) {
		UWorld* world = GetWorld();
		if (ensure(world)) {
			AFPPlayerController* localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
			if (ensure(localPlayer)) {
				AFireFighterPawn* localPawn = Cast<AFireFighterPawn>(localPlayer->GetPawn());
				if (localPawn == inPawn) {
					UE_LOG(LogTemp, Warning, TEXT("Local pawn joining"));
					// here we know this is local pawn and we are server, so update status manually and the player's join status should be host
					UpdatePlayerStatus(inPawn->GetFireFighterLobbyID(), EJoinStatus::Host);
					if (ensure(lobbyUI) && localPawn) {
						lobbyUI->ShowSelectedRole(localPawn->GetFireFighterLobbyRole());
					}
				}
			}
		}
		// update the widget manually on the server side
		if (ensure(lobbyUI)) {
			lobbyUI->UpdateDisplayedInfo(P0Status, P1Status, P2Status, P3Status, P4Status, P5Status);
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

void ALobbyManager::UpdatePlayerStatus(int32 playerID, FString inName, ERoleType inRole, EJoinStatus inStatus)
{
	// update all attributes of a specific player
	UpdatePlayerStatus(playerID, inName);
	UpdatePlayerStatus(playerID, inRole);
	UpdatePlayerStatus(playerID, inStatus);
}

void ALobbyManager::UpdatePlayerStatus(int32 playerID, FString inName)
{
	// update the player name of specific id
	switch (playerID)
	{
	case 0:
		P0Status.playerName = inName;
		break;
	case 1:
		P1Status.playerName = inName;
		break;
	case 2:
		P2Status.playerName = inName;
		break;
	case 3:
		P3Status.playerName = inName;
		break;
	case 4:
		P4Status.playerName = inName;
		break;
	case 5:
		P5Status.playerName = inName;
		break;
	default:
		break;
	}
}

void ALobbyManager::UpdatePlayerStatus(int32 playerID, ERoleType inRole)
{
	// update the player role of specific id
	switch (playerID)
	{
	case 0:
		P0Status.chosenRole = inRole;
		break;
	case 1:
		P1Status.chosenRole = inRole;
		break;
	case 2:
		P2Status.chosenRole = inRole;
		break;
	case 3:
		P3Status.chosenRole = inRole;
		break;
	case 4:
		P4Status.chosenRole = inRole;
		break;
	case 5:
		P5Status.chosenRole = inRole;
		break;
	default:
		break;
	}
}

void ALobbyManager::UpdatePlayerStatus(int32 playerID, EJoinStatus inStatus)
{
	// update the player koin status of specific id
	switch (playerID)
	{
	case 0:
		P0Status.joinStatus = inStatus;
		break;
	case 1:
		P1Status.joinStatus = inStatus;
		break;
	case 2:
		P2Status.joinStatus = inStatus;
		break;
	case 3:
		P3Status.joinStatus = inStatus;
		break;
	case 4:
		P4Status.joinStatus = inStatus;
		break;
	case 5:
		P5Status.joinStatus = inStatus;
		break;
	default:
		break;
	}
}

void ALobbyManager::Rep_LobbyPlayerStatus()
{
	// as any player status changes, update all the informations to the widgets
	if (ensure(lobbyUI)) {
		lobbyUI->UpdateDisplayedInfo(P0Status, P1Status, P2Status, P3Status, P4Status, P5Status);
	}
}

void ALobbyManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyManager, joinedPlayer);
	DOREPLIFETIME(ALobbyManager, joinedPawns);
	DOREPLIFETIME(ALobbyManager, P0Status);
	DOREPLIFETIME(ALobbyManager, P1Status);
	DOREPLIFETIME(ALobbyManager, P2Status);
	DOREPLIFETIME(ALobbyManager, P3Status);
	DOREPLIFETIME(ALobbyManager, P4Status);
	DOREPLIFETIME(ALobbyManager, P5Status);
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

