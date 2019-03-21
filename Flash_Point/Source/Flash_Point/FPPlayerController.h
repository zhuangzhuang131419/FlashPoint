// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"
#include "Engine/Engine.h"
#include "MenuSystem/FireFighterUI.h"
#include "GeneralTypes.h"
#include "FPPlayerController.generated.h"

class AFireFighterPawn;
class AVictim;
class AHazmat;
class AWall;
class ATile;
class ADoor;
class AEdgeUnit;
class AGameBoard;
class AChatManager;

/**
 * 
 */
UCLASS()
class FLASH_POINT_API AFPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFPPlayerController();

	// Below functions are for changing operations
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	EGameOperations GetCurrentOperation();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetOpenDoor();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetNone();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetPlaceFireFighter();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SetRespawnFireFighter();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetMove();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SetChopWall();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetExtinguishFire();
	UFUNCTION(BlueprintCallable, Category="GameOperations")
	void SetCarry();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void NotifyCarryVictim(bool isCarrying);
	UFUNCTION(Exec, BlueprintCallable, Category = "GameOperations")
	void SwitchRole(ERoleType inRole);
	// Cheating related
	UFUNCTION(BlueprintCallable, Category = "Cheating")
	bool ConsumptionOn();

	// Getter and setters for attributes
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void SetGameBoard(AGameBoard* inGame);
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	AGameBoard* GetGameBoard();
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	int32 GetTurnNum();
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void SetTurnNum(int32 turnNum);
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void NotifyPlayerTurn();
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void EndPlayerTurn();
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	UFireFighterStatus* GetFireFighterStatusBar(int32 id);
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void NotifyGameOver(bool isWin);
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void SetPlayerName(FString inName);
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	FString GetPlayerName();

	// Here are the server excuted functions to synchronize all status on each client connected
	// choped wall on server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChopWall(AWall* wall);
	// open door on server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOpenDoor(ADoor* door);
	// Move a pawn on server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMovePawn(ATile* tile, AFireFighterPawn* pawnToMove, const TArray<ATile*>& trace);
	// Place a pawn on server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPlacePawn(ATile* tile, AFireFighterPawn* pawnToPlace);
	// Extinguish fire on a certain tile to be done on server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerExtinguishFire(ATile* tile);
	// Server advance fire
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAdvanceFire(AGameBoard* board);
	// server replenish POI
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAdvancePOI(AGameBoard* board);
	// server flash over
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFlashOver(AGameBoard* board);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDropVictim(AFireFighterPawn * fireFighterPawn);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCarryVictim(AFireFighterPawn * fireFighterPawn);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDropHazmat(AFireFighterPawn * fireFighterPawn);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCarryHazmat(AFireFighterPawn * fireFighterPawn);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerGetFireFighterID(AFireFighterPawn * fireFighterPawn, AGameBoard* inGameBoard);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEndTurn(AGameBoard* inGameBoard);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAdjustAP(AFireFighterPawn* fireFighterPawn, int32 adjustAP);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendGlobalText(AChatManager* chatMan, const FString& message);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetFireFighterName(AFireFighterPawn* fireFighterPawn, const FString& inName);

	UFUNCTION(BlueprintCallable)
	void DropVictim();
	UFUNCTION(BlueprintCallable)
	void CarryVictim();

	UFUNCTION(BlueprintCallable)
	void DropHazmat();
	UFUNCTION(BlueprintCallable)
	void CarryHazmat();

	// FUNCTIONS for joining or creating games
	UFUNCTION(BlueprintCallable, Category = "Join Game")
	void FindGameBoard();
	UFUNCTION(BlueprintCallable, Category = "Role switch")
	void MakeBasicFireFighterUI();

	// cheating functions
	UFUNCTION(Exec, Category = "Cheating")
	void EnableAPConsumption(int32 flag);
	


protected:
	// FIELDS
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Player Attributes")
	AGameBoard* gameBoard = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Player Attributes")
	int32 myTurnNum = -1;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Player Attributes")
	FString playerName = "";

	// Cheating flags
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Cheating")
	bool apConsumptionOn = true;

	// UI related fields
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	UFireFighterUI* inGameUI = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireFighterUI> BasicFireFighterClass = nullptr;

	void FindChatUI();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	EGameOperations CurrentOperation = EGameOperations::None;
};
