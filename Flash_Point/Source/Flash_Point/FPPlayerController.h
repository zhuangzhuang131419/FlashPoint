// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UnrealNetwork.h"
#include "MenuSystem/FireFighterUI.h"
#include "GeneralTypes.h"
#include "FPPlayerController.generated.h"

class AFireFighterPawn;
class AVictim;
class AWall;
class ATile;
class ADoor;
class AEdgeUnit;
class AGameBoard;

/**
 * 
 */
UCLASS()
class FLASH_POINT_API AFPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
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
	AFPPlayerController();

	// Getter and setters for attributes
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	void SetGameBoard(AGameBoard* inGame);
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	AGameBoard* GetGameBoard();
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	int32 GetTurnNum();
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	void SetTurnNum(int32 turnNum);

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
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDrop(AFireFighterPawn * fireFighterPawn);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCarryVictim(AFireFighterPawn * fireFighterPawn);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerGetFireFighterID(AFireFighterPawn * fireFighterPawn, AGameBoard* inGameBoard);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEndTurn(AGameBoard* inGameBoard);

	UFUNCTION(BlueprintCallable)
	void DropVictim();
	UFUNCTION(BlueprintCallable)
	void CarryVictim();

	// FUNCTIONS for joining or creating games
	UFUNCTION(BlueprintCallable, Category = "Join Game")
	void FindGameBoard();
	UFUNCTION(BlueprintCallable, Category = "Role switch")
	void MakeBasicFireFighterUI();
	


protected:
	// FIELDS
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Player Attributes")
	AGameBoard* gameBoard = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Player Attributes")
	int32 myTurnNum = -1;

	// UI related fields
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	UFireFighterUI* inGameUI = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireFighterUI> BasicFireFighterClass = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	EGameOperations CurrentOperation = EGameOperations::None;
};
