// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralTypes.h"
#include "GameFramework/Actor.h"
#include "Ambulance.generated.h"

class AGameBoard;
class ATile;

UCLASS()
class FLASH_POINT_API AAmbulance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmbulance(const FObjectInitializer& ObjectInitializer);

	// The parking location of the ambulance
	FLocation parkingSpot1;
	FLocation parkingSpot2;

	AGameBoard* board;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ambulanceMesh = nullptr;
};
