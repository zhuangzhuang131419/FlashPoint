// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralTypes.h"
#include "GameFramework/Actor.h"
#include "Ambulance.generated.h"

class AGameBoard;
class ATile;
class AVictim;

UCLASS()
class FLASH_POINT_API AAmbulance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmbulance();


	// The parking location of the ambulance
	FLocation parkingSpot1;
	FLocation parkingSpot2;

	AGameBoard* board;

	UPROPERTY(BlueprintReadWrite, Category = "Position Info")
	int32 currentPosition = -1;

	UFUNCTION(BlueprintCallable, Category = "Position")
	void SetAmbulancePosition(int32 newPos);

	UFUNCTION(BlueprintCallable, Category = "Position")
	int32 GetAmbulancePosition();

	void RescueVictims(TArray<AVictim*>* victims, ATile* currentTile);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ambulanceMesh = nullptr;


};
