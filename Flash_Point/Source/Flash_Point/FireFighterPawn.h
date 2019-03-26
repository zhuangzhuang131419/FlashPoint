// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine.h"
#include "GeneralTypes.h"
#include "UnrealNetwork.h"
#include "FireFighterPawn.generated.h"

class ATile;
class AEdgeUnit;
class AVictim;
class AHazmat;
class AGameBoard;
class AFPPlayerController;

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
	int32 GetCommandAP() { return commandAP; }
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetExtinguishAP() { return extinguishAP; }

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
	// chceking if a operation can be performed by the pawn
	bool CheckCanExtinguish(int32 baseCost);
	bool IsAdjacentToWall(AEdgeUnit* inEdge);

	// Getter and setters for firefighter pawn id
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetFireFighterID();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetFireFighterID(int32 inID);
	// getter and setter for victim
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	AVictim* GetVictim();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetVictim(AVictim* victim);
	// getter and setter for hazmat
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	AHazmat* GetHazmat();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetHazmat(AHazmat* hazmat);
	// Getter and setter for gameboard
	AGameBoard* GetPlayingBoard() { return playingBoard; }

	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void InitializeFireFighter();
	// a function for setting fire fighter material
	UFUNCTION(BlueprintImplementableEvent, Category = "Firefighter Attributes")
	void FireFighterSwitchColor(int32 numID);
	UFUNCTION(BlueprintImplementableEvent, Category = "Firefighter Attributes")
	void ShowControllable(bool isLocalControl, bool isControlled);
	void KnockDown();

protected:
	// current firefighter position
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Firefighter Attributes")
	ATile* placedOn = nullptr;
	// The firefighter's current ap value and max ap value
	UPROPERTY(ReplicatedUsing = Rep_APChanges, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 currentAP = 4;	// initial AP for all firefighters are 4
	UPROPERTY(ReplicatedUsing = Rep_APChanges, BlueprintReadWrite, EditDefaultsOnly, Category = "Firefighter Attributes")
	int32 maxAP = 8;		// Firefighter max ap
	int32 extinguishAP = 0; // Extinguish AP 
	int32 movementAP = 0;	// Movingment AP
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
	UPROPERTY(ReplicatedUsing = Rep_PawnID, BlueprintReadWrite, VisibleAnyWhere, Category = "Firefighter Attributes")
	int32 fireFighterID = -1;
	UPROPERTY(ReplicatedUsing = Rep_RoleType, BlueprintReadWrite, VisibleAnyWhere, Category = "Firefighter Attributes")
	ERoleType fireFighterRole = ERoleType::Basic;
	UPROPERTY(ReplicatedUsing = Rep_FireFighterknockDownRelocate, BlueprintReadWrite, VisibleAnyWhere, Category = "Firefighter Attributes")
	bool relocationFlag = true;
	AGameBoard* playingBoard = nullptr;
	UPROPERTY(VisibleAnyWhere, Category = "Firefighter Attributes")
	AFPPlayerController* myOwner = nullptr;
	// UI for visualizing the teammate's firefighter's status
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Firefighter Attributes")
	class UFireFighterStatus* statusBar = nullptr;
	// Name of the firefighter's owner
	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadWrite, Category = "Firefighter Attributes")
	FString fireFighterName = "";

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
	void Rep_FireFighterknockDownRelocate();

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
	AVictim* victim = nullptr;
	UPROPERTY(VisibleAnyWhere, Category = "Firefighter Attributes")
	AHazmat* hazmat = nullptr;
};