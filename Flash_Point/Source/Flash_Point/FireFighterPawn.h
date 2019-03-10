// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "FireFighterPawn.generated.h"

class ATile;
class AVictim;
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
	// Getter and setter for the firefighter's current location
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	ATile* GetPlacedOn();
	void SetPlacedOn(ATile* tile);
	// Getter and setter for current firefighter ap
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetCurrentAP();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetCurrentAP(int32 current);
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
	int32 GetExtinguishConsumption();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetExtinguishConsumption(int32 current);
	// chceking if a operation can be performed by the pawn
	bool CheckCanExtinguish(int32 baseCost);
	// Getter and setters for firefighter pawn id
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	int32 GetFireFighterID();
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void SetFireFighterID(int32 inID);
	UFUNCTION(BlueprintCallable, Category = "Firefighter Attributes")
	void InitializeFireFighter();

	void KnockDown();

protected:
	// current firefighter position
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Firefighter Attributes")
	ATile* placedOn = nullptr;
	// The firefighter's current ap value and max ap value
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 currentAP = 4;	// initial AP for all firefighters are 4
	UPROPERTY(replicated, BlueprintReadWrite, EditDefaultsOnly, Category = "Firefighter Attributes")
	int32 maxAP = 8;	// Firefighter max ap
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 moveConsumption = 1;
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 openConsumption = 1;
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 extinguishConsumption = 1;
	UPROPERTY(ReplicatedUsing = Rep_PawnID, BlueprintReadWrite, VisibleAnyWhere, Category = "Firefighter Attributes")
	int32 fireFighterID = -1;
	AGameBoard* playingBoard = nullptr;
	AFPPlayerController* myOwner = nullptr;


	// REPLICATION FUNCTIONS
	UFUNCTION()
	void Rep_PawnID();


	// Overriding setting all lifetime replicates function
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	bool CarrayVictim(AVictim* Citizen);

	UFUNCTION(BlueprintCallable, Category = "Citizen Carraying")
	AVictim* GetVictim();
	UFUNCTION(BlueprintCallable, Category = "Citizen Carraying")
	void SetVictim(AVictim* victim);

protected:
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Firefighter Attributes")
	AVictim* victim = nullptr;
};