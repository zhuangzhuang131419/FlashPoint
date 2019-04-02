// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealNetwork.h"
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
	// getter and setter for the crew manager
	ACrewManager* GetCrewManager();
	void SetCrewManager(ACrewManager* inCrewMan);
	// binding a lobby UI with the manager
	void BindLobbyUI(ULobbyUI* inLobbyUI);

protected:
	// REPLICATED FIELDS
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Lobby Creation")
	int32 joinedPlayer = 0;	// number of players joined the game
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Lobby Creation")
	TArray<AFireFighterPawn*> joinedPawns;	// all pawns currently in the game
	
	// FIELDS
	// A crew manager managing role switching
	ACrewManager* crewMan = nullptr;
	ULobbyUI* lobbyUI = nullptr;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
