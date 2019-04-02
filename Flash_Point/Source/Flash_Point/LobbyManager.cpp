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

void ALobbyManager::PlayerReadyStart()
{
	// if is server, check join status and do the server travel
	if (HasAuthority()) {
		// TODO do server travel to desired level
	}
	else {
		// change the ready status of the local player in lobby
		UWorld* world = GetWorld();
		if (ensure(world)) {
			AFPPlayerController* localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
			if (!ensure(localPlayer)) return;
			AFireFighterPawn* localPawn = Cast<AFireFighterPawn>(localPlayer->GetPawn());
			if (ensure(localPawn)) {
				localPlayer->ServerChangeReadyStatus(this, localPawn);
			}
		}
	}
}

void ALobbyManager::UpdatePlayerJoinStatus(AFireFighterPawn * inPawn, EJoinStatus inStatus)
{
	if (ensure(inPawn)) {
		UpdatePlayerStatus(inPawn->GetFireFighterLobbyID(), inStatus);
	}
	// since this should be executed on the server, update server display manually
	if (HasAuthority()) {
		if (ensure(lobbyUI)) {
			lobbyUI->UpdateDisplayedInfo(P0Status, P1Status, P2Status, P3Status, P4Status, P5Status);
			lobbyUI->ChangeJoinStartButtonStatus("Start", IsAllPlayerReady());
		}
	}
}

void ALobbyManager::UpdatePlayerStatus(int32 playerID, FPlayerLobbyInfo inInfo)
{
	// update the player name of specific id
	switch (playerID)
	{
	case 0:
		P0Status = inInfo;
		break;
	case 1:
		P1Status = inInfo;
		break;
	case 2:
		P2Status = inInfo;
		break;
	case 3:
		P3Status = inInfo;
		break;
	case 4:
		P4Status = inInfo;
		break;
	case 5:
		P5Status = inInfo;
		break;
	default:
		break;
	}
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

bool ALobbyManager::IsAllPlayerReady()
{
	if (P0Status.joinStatus == EJoinStatus::Waiting) {
		return false;
	}
	else if (P1Status.joinStatus == EJoinStatus::Waiting) {
		return false;
	}
	else if (P2Status.joinStatus == EJoinStatus::Waiting) {
		return false;
	}
	else if (P3Status.joinStatus == EJoinStatus::Waiting) {
		return false;
	}
	else if (P4Status.joinStatus == EJoinStatus::Waiting) {
		return false;
	}
	else if (P5Status.joinStatus == EJoinStatus::Waiting) {
		return false;
	}
	if (joinedPawns.Num() >= 3) {
		return true;
	}
	else {
		return false;
	}
}

void ALobbyManager::ShiftLobbyInfo(int32 fromID, int32 toID)
{
	// get the old information of the specified ID
	FPlayerLobbyInfo tempInfo = GetPlayerLobbyInfo(fromID);
	UpdatePlayerStatus(toID, tempInfo);
	// clear the slot taken by the player before
	UpdatePlayerStatus(fromID, FPlayerLobbyInfo());
}

FPlayerLobbyInfo ALobbyManager::GetPlayerLobbyInfo(int32 playerID)
{
	// update the player koin status of specific id
	switch (playerID)
	{
	case 0:
		return P0Status;
		break;
	case 1:
		return P1Status;
		break;
	case 2:
		return P2Status;
		break;
	case 3:
		return P3Status;
		break;
	case 4:
		return P4Status;
		break;
	case 5:
		return P5Status;
		break;
	default:
		return FPlayerLobbyInfo();
		break;
	}
}

void ALobbyManager::ExitLobby(AFireFighterPawn * inPawn)
{
	if (!ensure(inPawn)) return;
	// Delete the joined firefighter from the lobby's record
	joinedPawns.Remove(inPawn);
	// get the inPawn's ID befor removing
	int32 tempID = inPawn->GetFireFighterLobbyID();
	// shift all other firefighter's lobby ID
	for (AFireFighterPawn* tempPawn : joinedPawns) {
		if (tempPawn->GetFireFighterLobbyID() >= tempID) {
			
		}
	}
}

void ALobbyManager::LobbyManagerSelectRole(AFireFighterPawn * inPawn, ERoleType inRole)
{
	if (!ensure(inPawn) || !ensure(crewMan)) return;
	crewMan->SelectRoleForFirefighter(inPawn, inRole);
	// update all status in the lobby
	UpdatePlayerStatus(inPawn->GetFireFighterLobbyID(), inPawn->GetFireFighterLobbyRole());
	// if the pawn is local pawn on server, manually update UI
	if (HasAuthority()) {
		UWorld* world = GetWorld();
		if (ensure(world)) {
			AFPPlayerController* localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
			if (ensure(localPlayer)) {
				AFireFighterPawn* localPawn = Cast<AFireFighterPawn>(localPlayer->GetPawn());
				if (localPawn == inPawn) {
					UE_LOG(LogTemp, Warning, TEXT("Local pawn joining"));
					// here we know this is local pawn and we are server, so update status manually and the player's join status should be host
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

void ALobbyManager::Rep_LobbyPlayerStatus()
{
	// as any player status changes, update all the informations to the widgets
	if (!ensure(lobbyUI)) return;
	lobbyUI->UpdateDisplayedInfo(P0Status, P1Status, P2Status, P3Status, P4Status, P5Status);

	// check the new join status of current player in lobby to update button text
	UWorld* world = GetWorld();
	if (ensure(world)) {
		AFPPlayerController* localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
		if (!ensure(localPlayer)) return;
		AFireFighterPawn* localPawn = Cast<AFireFighterPawn>(localPlayer->GetPawn());
		if (ensure(localPawn)) {
			FPlayerLobbyInfo tempInfo = GetPlayerLobbyInfo(localPawn->GetFireFighterLobbyID());
			if (tempInfo.joinStatus == EJoinStatus::Ready) {
				lobbyUI->ChangeJoinStartButtonStatus("Cancel", true);
			}
			else if (tempInfo.joinStatus == EJoinStatus::Waiting) {
				lobbyUI->ChangeJoinStartButtonStatus("Ready", true);
			}
		}
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

