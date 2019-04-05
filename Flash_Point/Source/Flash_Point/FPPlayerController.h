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
class ACrewManager;
class UOptionPrompt;
class APOI;
class ALobbyManager;
class ULobbyUI;
class AAmbulance;
class AFireEngine;

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
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SetCurrentOperation(EGameOperations current) { CurrentOperation = current; }
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
	void SetDriveAmbulance();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	bool SetGetOutAmbulance();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	bool SetGetOutFireEngine();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SetDriveFireEngine();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SetRadio();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SetFireDeckGun();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SetFlip();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SetDodge();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SetSqueeze();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SetCommand();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void CancelCommand();

	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void NotifyCarryVictim(bool isCarrying);
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void NotifyLeadVictim(bool isCarrying);
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void NotifyGameForcedStop();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void NotifyCarryHazmat(bool isCarrying);

	UFUNCTION(Exec, BlueprintCallable, Category = "GameOperations")
	void SwitchRole(ERoleType inRole);
	UFUNCTION(Exec, BlueprintCallable, Category = "GameOperations")
	void SelectRole(ERoleType inRole, AFireFighterPawn* inPawn);
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void SwitchRoleWidget(ERoleType inRole);
	// Cheating related
	UFUNCTION(BlueprintCallable, Category = "Cheating")
	bool ConsumptionOn();

	// Getter and setters for attributes
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void SetGameBoard(AGameBoard* inGame);
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	AGameBoard* GetGameBoard();
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void SetLobbyManager(ALobbyManager* inMan);
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	ALobbyManager* GetLobbyManager();
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	int32 GetTurnNum();
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void SetTurnNum(int32 turnNum);
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void NotifyPlayerTurn();
	UFUNCTION(Exec, BlueprintCallable, Category = "GameOperations")
	void NotifyPlayerDodge();
	UFUNCTION(BlueprintCallable, Category = "GameOperations")
	void NotifyReRoll();
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
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void EnableOperations(bool enable);
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void PromtCrewChange();
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void PromtCommandStatus(EAcceptanceStatus commandStatus);
	UFUNCTION(BlueprintCallable, Category = "Take Turn")
	void PromtOptionPanel(EOptionPromptType option, FString optionText);

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
	void ServerHealVictim(AFireFighterPawn * fireFighterPawn, AVictim* targetVictim);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerGetFireFighterID(AFireFighterPawn * fireFighterPawn, AGameBoard* inGameBoard);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEndTurn(AGameBoard* inGameBoard);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAdjustAP(AFireFighterPawn* fireFighterPawn, int32 adjustAP);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAdjustCAFSAP(AFireFighterPawn* fireFighterPawn, int32 adjustAP);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAdjustCommandAP(AFireFighterPawn* captain, int32 adjustAP);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAdjustRescueSpecAP(AFireFighterPawn* fireFighterPawn, int32 adjustAP);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendGlobalText(AChatManager* chatMan, const FString& message);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetFireFighterName(AFireFighterPawn* fireFighterPawn, const FString& inName);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSwitchRole(ACrewManager* inCrewMan, AFireFighterPawn* fireFighterPawn, ERoleType inRole);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSelectRole(AFireFighterPawn* inPawn, ALobbyManager* inMan, ERoleType toRole);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSolveKnockDown(AGameBoard* board);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSolveHazmatExplosions(AGameBoard* board);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpecialistTurnAdjust(AFireFighterPawn* fireFighterPawn);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCommandDoorOperation(AFireFighterPawn* fireFighterPawn, AFireFighterPawn* captain, ADoor* target);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCommandTileOperation(AFireFighterPawn* fireFighterPawn, AFireFighterPawn* captain, const TArray<ATile*>& targets);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetCommandStatus(AFireFighterPawn* captain, EAcceptanceStatus inStatus);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerJoinLobby(ALobbyManager* inMan, AFireFighterPawn* inPawn, const FString& inName);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChangeReadyStatus(ALobbyManager* inMan, AFireFighterPawn* inPawn);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerExitFromLobby(ALobbyManager* inMan, AFireFighterPawn* inPawn);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerExitFromBoard(AGameBoard* inBoard, AFireFighterPawn* inPawn);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerGetInCar(AFireFighterPawn * inPawn);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerGetOutAmbulance(AFireFighterPawn * inPawn, ATile* current, AAmbulance* inAmbulance);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerGetOutFireEngine(AFireFighterPawn * inPawn, ATile* current, AFireEngine* inFireEngine);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFireDeckGun(AFireFighterPawn * inPawn, ATile* targetTile);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveAmbulance(AAmbulance* localAmbulance, ATile* currentTile);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveFireEngine(AFireEngine* localEngine, ATile* currentTile);

	UFUNCTION(BlueprintCallable)
	void DropVictim();
	UFUNCTION(BlueprintCallable)
	void CarryVictim();

	UFUNCTION(BlueprintCallable)
	void DropHazmat();
	UFUNCTION(BlueprintCallable)
	void CarryHazmat();

	UFUNCTION(BlueprintCallable)
	void HealVictim();

	UFUNCTION(BlueprintCallable)
	bool GetInAmbulance();
	UFUNCTION(BlueprintCallable)
	void GetOutAmbulance();
	UFUNCTION(BlueprintCallable)
	bool GetInFireEngine();
	UFUNCTION(BlueprintCallable)
	void GetOutFireEngine();

	UFUNCTION(BlueprintCallable)
	void RemoveHazmat();

	void Dodge();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerRevealPOI(ATile* targetTile);

	// Getter and setter for player commanded firefighter
	AFireFighterPawn* GetCommanded();
	void SetCommanded(AFireFighterPawn* inFireFighter);

	// FUNCTIONS for joining or creating games
	UFUNCTION(BlueprintCallable, Category = "Join Game")
	void FindGameBoard();
	UFUNCTION(BlueprintCallable, Category = "Join Game")
	void FindLobbyManager();
	UFUNCTION(BlueprintCallable, Category = "Role switch")
	void RelateInGameUI(AFireFighterPawn * fireFighterPawn);
	UFUNCTION(BlueprintCallable, Category = "Role switch")
	void MakeBasicFireFighterUI();
	UFUNCTION(BlueprintCallable, Category = "Option Prompt")
	void MakeOptionPromptUI();

	// cheating/testing functions
	UFUNCTION(Exec, Category = "Cheating")
	void EnableAPConsumption(int32 flag);
	UFUNCTION(Exec, Category = "Save Load Related")
	void SaveCurrentGame(FString saveName);
	
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	UFireFighterUI* inGameUI = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	ULobbyUI* lobbyUI = nullptr;


protected:
	// FIELDS
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Player Attributes")
	AGameBoard* gameBoard = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Player Attributes")
	ALobbyManager* lobbyMan = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Player Attributes")
	ACrewManager* crewMan = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Player Attributes")
	int32 myTurnNum = -1;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Player Attributes")
	FString playerName = "";
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Command Related")
	AFireFighterPawn* commandedPawn = nullptr;

	// Cheating flags
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Cheating")
	bool apConsumptionOn = true;

	// UI related fields
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	UOptionPrompt* OptionPromptUI = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireFighterUI> BasicFireFighterClass = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireFighterUI> CAFSFireFighterClass = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireFighterUI> DriverClass = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireFighterUI> CaptainClass = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireFighterUI> GeneralistClass = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireFighterUI> HazTecClass = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireFighterUI> ImageTecClass = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireFighterUI> ParamedicClass = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireFighterUI> RescueSpecClass = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireFighterUI> RescueDogClass = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireFighterUI> VeteranClass = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UOptionPrompt> OptionClass = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<ULobbyUI> LobbyUIClass = nullptr;

	void FindChatUI();
	void FindCrewManager();

	void JoinGameLobby();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	EGameOperations CurrentOperation = EGameOperations::None;
};
