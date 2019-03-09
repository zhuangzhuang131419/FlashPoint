// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "FireFighterPawn.generated.h"

class ATile;
class AVictim;

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

protected:
	// current firefighter position
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Firefighter Attributes")
	ATile* placedOn = nullptr;
	// The firefighter's current ap value and max ap value
	UPROPERTY(replicated, BlueprintReadWrite, EditAnyWhere, Category = "Firefighter Attributes")
	int32 currentAP = 4;	// initial AP for all firefighters are 4
	UPROPERTY(replicated, BlueprintReadWrite, EditDefaultsOnly, Category = "Firefighter Attributes")
	int32 maxAP = 8;	// Firefighter max ap


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
	AVictim* victim = nullptr;
};