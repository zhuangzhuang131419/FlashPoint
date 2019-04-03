// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine.h"
#include "GeneralTypes.h"
#include "UnrealNetwork.h"
#include "FireFighterPawn.generated.h"

class ATile;
class ADoor;
class AEdgeUnit;
class AVictim;
class AHazmat;
class AGameBoard;
class AFPPlayerController;
class ACrewManager;
class ALobbyManager;
class ULobbyUI;

const static int32 FLAG_LEAVE_LOBBY = -100;

UCLASS()
class FLASH_POINT_API AFireFighterPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFireFighterPawn();

	// A blueprint event to relocate the camera
	UFUNCTION(BlueprintImplementableEvent, Category = "Camera Management")
	void RelocateCamera(FVector location);
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void BindStatusWidget();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void RelateAllFireFighterStatus();
	// Getter and setter for the firefighter's current location
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	ATile* GetPlacedOn();
	void SetPlacedOn(ATile* tile);
	// Getter and setter for current firefighter ap
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetCurrentAP();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetMovementAP() { return movementAP; }
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetMovementAP(int32 inAP) { movementAP = inAP; }
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetCommandAP() { return commandAP; }
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetCommandAP(int32 inAP) { commandAP = inAP; }
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetExtinguishAP() { return extinguishAP; }
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetExtinguishAP(int32 inAP) { extinguishAP = inAP; }
	// Function for checking if the firefighter is with engine or ambulance
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	bool IsWithEngine();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	bool IsWithAmbulance();

	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetCurrentAP(int32 current);
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetMaxAP();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetRestoreAP();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void AdjustFireFighterAP(int32 adjustAP);
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void AdjustCAFSFireFighterExtinguishAP(int32 adjustAP);
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void AdjustSpecialistMoveAP(int32 adjustAP);

	// Getter and setter for the firefighter's name
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	FString GetFireFighterName();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetFireFighterName(FString inName);
	// Getter and setter for the firefighter's role
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	ERoleType GetFireFighterRole();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetFireFighterRole(ERoleType inRole);
	// Getter and setter for the firefighter's lobby role
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	ERoleType GetFireFighterLobbyRole();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetFireFighterLobbyRole(ERoleType inRole);
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void AdjustRoleProperties(ERoleType inRole);
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void ResetProperties();
	// getter and setter for all consumptions
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetMoveConsumption();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetMoveConsumption(int32 current);
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetOpenConsumption();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetOpenConsumption(int32 current);
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetChopConsumption();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetChopConsumption(int32 current);
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetExtinguishConsumption();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetExtinguishConsumption(int32 current);
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetCrewChangeConsumption();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetDodgeConsumption(int32 current) { dodgeConsumption = current; }
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetDodgeConsumption() { return dodgeConsumption; }
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetHealConsumption() { return healConsumption; }
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetSequeezeConsumption() { return sequeezeConsumption; }
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetSequeezeConsumption(int32 current) { sequeezeConsumption = current; }
	int32 GetFireDeckGunConsumption(){return deckGunConsumption;}

	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetFlipConsumption() { return flipConsumption; }
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetFlipConsumption(int32 current) { flipConsumption = current; }

	// chceking if a operation can be performed by the pawn
	bool CheckCanExtinguish(int32 baseCost);
	bool IsAdjacentToWall(AEdgeUnit* inEdge);
	bool CheckIsVicinty(ATile* veteran);

	// Getter and setters for firefighter pawn id
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetFireFighterID();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetFireFighterID(int32 inID);
	// Getter and setters for firefighter lobby id
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetFireFighterLobbyID();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetFireFighterLobbyID(int32 inID);
	// getter and setter for carried victim
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	AVictim* GetCarriedVictim() { return carriedVictim; }
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetCarriedVictim(AVictim* victim);

	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	AVictim* GetVictim();

	// getter and setter for leading victim
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	AVictim* GetLeading();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetLeading(AVictim* victim);
	// getter and setter for hazmat
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	AHazmat* GetHazmat();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetHazmat(AHazmat* hazmat);
	// Getter and setter for gameboard
	AGameBoard* GetPlayingBoard() { return playingBoard; }

	// Getter and setter for can dodge
	bool GetCanDodge();
	bool canDodgeAcross(AEdgeUnit * targetEdge);
	void SetDodgeAbility(bool current) { dodgeAbility = current; }

	// Getter and setter for hasGainedAPThisTurn
	bool HasGainedAPThisTurn() { return hasGainedAPThisTurn; }
	void SetHasGainedAPThisTurn(bool current) { hasGainedAPThisTurn = current; }

	bool IsInCar() { return isInCar; }
	void SetIsInCar(bool current) { isInCar = current; }

	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void InitializeFireFighter();
	// a function for setting fire fighter material
	UFUNCTION(BlueprintImplementableEvent, Category = "Firefighter Attributes")
	void FireFighterSwitchColor(int32 numID);
	void KnockDown();
	
	// Commanding related functions
	UFUNCTION(BlueprintImplementableEvent, Category = "Firefighter Attributes")
	void ShowControllable(bool isLocalControl, bool isControlled);
	UFUNCTION(BlueprintImplementableEvent, Category = "Firefighter Attributes")
	void ShowIsCommanded(bool isCommanded);
	void SetIsCommanded(bool isCommanded) { this->isCommanded = isCommanded; }
	bool GetIsCommanded() { return isCommanded; }
	void CommandDoorOperation(ADoor* target, AFireFighterPawn* commander);
	void CommandTileOperation(TArray<ATile*> targets, AFireFighterPawn* commander);
	void AcceptMoveCommand(bool accepted);
	void AcceptDoorCommand(bool accepted);
	// getter and setter for command acceptance
	EAcceptanceStatus GetCommandAcceptance() { return commandAcceptance; }
	void SetCommandAcceptance(EAcceptanceStatus inAcc) { commandAcceptance = inAcc; }
	void FlipServerDodgeFlag() { serverDodgeFlag = !serverDodgeFlag; }
	void SetVisibility(bool status);

	// function for binding the lobby UI with this firefighter
	void BindLobbyUIFirefighter(ULobbyUI* inLobbyUI);

	// Field
	

protected:
	// current firefighter position
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Firefighter Attributes")
	ATile* placedOn = nullptr;
	// The firefighter's current ap value and max ap value
	UPROPERTY(ReplicatedUsing = Rep_APChanges, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 currentAP = 0;	// initial AP for all firefighters are 0
	UPROPERTY(ReplicatedUsing = Rep_APChanges, BlueprintReadWrite, EditDefaultsOnly, Category = "Firefighter Attributes")
	int32 maxAP = 8;		// Firefighter max ap
	UPROPERTY(replicated, BlueprintReadWrite, EditDefaultsOnly, Category = "Firefighter Attributes")
	int32 extinguishAP = 0; // Extinguish AP 
	UPROPERTY(replicated, BlueprintReadWrite, EditDefaultsOnly, Category = "Firefighter Attributes")
	int32 movementAP = 0;	// Movingment AP
	UPROPERTY(replicated, BlueprintReadWrite, EditDefaultsOnly, Category = "Firefighter Attributes")
	int32 commandAP = 0;	// Command AP
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 restoreAP = 4;	// AP for restoration for firefighters are initialized to 4
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 moveConsumption = 1;
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 openConsumption = 1;
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 chopConsumption = 2;
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 extinguishConsumption = 1;
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 deckGunConsumption = 4;
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 crewChangeConsumption = 2;
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 dodgeConsumption = 2;
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 healConsumption = 1;
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 sequeezeConsumption = 1;
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 flipConsumption = 1;
	UPROPERTY(ReplicatedUsing = Rep_PawnID, BlueprintReadWrite, VisibleAnyWhere, Category = "Firefighter Attributes")
	int32 fireFighterID = -1;
	UPROPERTY(ReplicatedUsing = Rep_LobbyPawnID, BlueprintReadWrite, VisibleAnyWhere, Category = "Firefighter Attributes")
	int32 lobbyPlayerID = -1;
	UPROPERTY(ReplicatedUsing = Rep_RoleType, BlueprintReadWrite, VisibleAnyWhere, Category = "Firefighter Attributes")
	ERoleType fireFighterRole = ERoleType::Basic;
	UPROPERTY(ReplicatedUsing = Rep_LobbyRole, BlueprintReadWrite, VisibleAnyWhere, Category = "Firefighter Attributes")
	ERoleType lobbyRole = ERoleType::Basic;
	UPROPERTY(ReplicatedUsing = Rep_FireFighterknockDownRelocate, BlueprintReadWrite, VisibleAnyWhere, Category = "Firefighter Attributes")
	bool relocationFlag = true;
	UPROPERTY(ReplicatedUsing = Rep_FireFighterDodge, BlueprintReadWrite, VisibleAnyWhere, Category = "Firefighter Attributes")
	bool serverDodgeFlag = true;
	UPROPERTY(ReplicatedUsing = Rep_NotifyCommandedDoor, BlueprintReadWrite, VisibleAnyWhere, Category = "Commanding Related")
	ADoor* orderedDoor = nullptr;
	UPROPERTY(ReplicatedUsing = Rep_NotifyCommandedTiles, BlueprintReadWrite, VisibleAnyWhere, Category = "Commanding Related")
	TArray<ATile*> orderedTiles;
	// the captain who is commanding the current firefighter
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Commanding Related")
	AFireFighterPawn* captain = nullptr;
	// A value to indicate the status of the command given out
	UPROPERTY(ReplicatedUsing = Rep_CommandStatus, VisibleAnyWhere, BlueprintReadWrite, Category = "Commanding Related")
	EAcceptanceStatus commandAcceptance = EAcceptanceStatus::Empty;
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Player Attributes")
	bool dodgeAbility = false;
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Player Attributes")
	bool isVicinity = false;
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Player Attributes")
	bool isInCar = false;
	UPROPERTY(VisibleAnyWhere, Category = "Player Attributes")
	bool hasGainedAPThisTurn = false;
	AGameBoard* playingBoard = nullptr;
	ALobbyManager* lobbyMan = nullptr;
	UPROPERTY(VisibleAnyWhere, Category = "Firefighter Attributes")
	AFPPlayerController* myOwner = nullptr;
	// UI for visualizing the teammate's firefighter's status
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Firefighter Attributes")
	class UFireFighterStatus* statusBar = nullptr;
	// UI for in lobby operations
	ULobbyUI* lobbyUI = nullptr;
	// Name of the firefighter's owner
	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadWrite, Category = "Firefighter Attributes")
	FString fireFighterName = "";
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Commanding Related")
	bool isCommanded = false;

	// REPLICATION FUNCTIONS
	UFUNCTION()
	void Rep_PawnID();
	UFUNCTION()
	void Rep_RoleType();
	UFUNCTION()
	void Rep_APChanges();
	UFUNCTION()
	void Rep_CarryingVictim();
	UFUNCTION()
	void Rep_LeadingVictim();
	UFUNCTION()
	void Rep_FireFighterknockDownRelocate();
	UFUNCTION()
	void Rep_FireFighterDodge();
	UFUNCTION()
	void Rep_NotifyCommandedDoor();
	UFUNCTION()
	void Rep_NotifyCommandedTiles();
	UFUNCTION()
	void Rep_CommandStatus();
	UFUNCTION()
	void Rep_LobbyPawnID();
	UFUNCTION()
	void Rep_LobbyRole();

	// CURSOR BINDING FUNCTIONS
	UFUNCTION(BlueprintCallable, Category = "Command Related")
	void OnOverFirefighter(AActor* TouchedActor);
	UFUNCTION(BlueprintCallable, Category = "Command Related")
	void OnClickedFirefighter(AActor* TouchedActor, FKey ButtonPressed);
	UFUNCTION(BlueprintCallable, Category = "Command Related")
	void OnLeftFirefighter(AActor* TouchedActor);

	// Overriding setting all lifetime replicates function
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(ReplicatedUsing = Rep_CarryingVictim, VisibleAnyWhere, Category = "Firefighter Attributes")
	AVictim* carriedVictim = nullptr;
	UPROPERTY(ReplicatedUsing = Rep_LeadingVictim, VisibleAnyWhere, Category = "Firefighter Attributes")
	AVictim* leadVictim = nullptr;
	UPROPERTY(VisibleAnyWhere, Category = "Firefighter Attributes")
	AHazmat* hazmat = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Firefighter Attributes")
	USkeletalMeshComponent* FireFighter = nullptr;
};