// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealNetwork.h"
#include "GeneralTypes.h"
#include "LobbyManager.generated.h"

class AFireFighterPawn;
class ACrewManager;
class ULobbyUI;

UCLASS()
class FLASH_POINT_API ALobbyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALobbyManager();

	// Assigning realted fields to the firefighter
	UFUNCTION(BlueprintCallable, Category = "Lobby Joining")
	void AutoJoinLobby(AFireFighterPawn* inPawn, FString playerName);	
	// Selecting role using the lobby manager
	void LobbyManagerSelectRole(AFireFighterPawn* inPawn, ERoleType inRole);

	// getter and setter for the crew manager
	ACrewManager* GetCrewManager();
	void SetCrewManager(ACrewManager* inCrewMan);
	// binding a lobby UI with the manager
	void BindLobbyUI(ULobbyUI* inLobbyUI);

protected:

	// FUNCTIONS
	void UpdatePlayerStatus(int32 playerID, FString inName, ERoleType inRole, EJoinStatus inStatus);
	void UpdatePlayerStatus(int32 playerID, FString inName);
	void UpdatePlayerStatus(int32 playerID, ERoleType inRole);
	void UpdatePlayerStatus(int32 playerID, EJoinStatus inStatus);
	// this will only be called by the host who is the server to check if the game could start
	bool IsAllPlayerReady();
	// get the realated status to the player in lobby
	FPlayerLobbyInfo GetPlayerLobbyInfo(int32 playerID);

	// REPLICATED FIELDS
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Lobby Creation")
	int32 joinedPlayer = 0;	// number of players joined the game
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Lobby Creation")
	TArray<AFireFighterPawn*> joinedPawns;	// all pawns currently in the game
	// There are 6 of these info structs for 6 joinable slots
	UPROPERTY(ReplicatedUsing = Rep_LobbyPlayerStatus, VisibleAnyWhere, Category = "Lobby Creation")
	FPlayerLobbyInfo P0Status;
	UPROPERTY(ReplicatedUsing = Rep_LobbyPlayerStatus, VisibleAnyWhere, Category = "Lobby Creation")
	FPlayerLobbyInfo P1Status;
	UPROPERTY(ReplicatedUsing = Rep_LobbyPlayerStatus, VisibleAnyWhere, Category = "Lobby Creation")
	FPlayerLobbyInfo P2Status;
	UPROPERTY(ReplicatedUsing = Rep_LobbyPlayerStatus, VisibleAnyWhere, Category = "Lobby Creation")
	FPlayerLobbyInfo P3Status;
	UPROPERTY(ReplicatedUsing = Rep_LobbyPlayerStatus, VisibleAnyWhere, Category = "Lobby Creation")
	FPlayerLobbyInfo P4Status;
	UPROPERTY(ReplicatedUsing = Rep_LobbyPlayerStatus, VisibleAnyWhere, Category = "Lobby Creation")
	FPlayerLobbyInfo P5Status;
	
	// FIELDS
	// A crew manager managing role switching
	ACrewManager* crewMan = nullptr;
	ULobbyUI* lobbyUI = nullptr;

	// REPLICATION FUNCIONTS
	UFUNCTION()
	void Rep_LobbyPlayerStatus();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
