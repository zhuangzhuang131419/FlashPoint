// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralTypes.h"
#include "GameFramework/Actor.h"
#include "FireEngine.generated.h"

class ATile;
class AGameBoard;

UCLASS()
class FLASH_POINT_API AFireEngine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireEngine();

	void FireDeckGun();

	// The parking location of the fire engine
	FLocation parkingSpot1;
	FLocation parkingSpot2;

	UPROPERTY(BlueprintReadWrite, Category = "Position Info")
	int32 currentPosition = -1;

	UFUNCTION(BlueprintCallable, Category = "Position")
	void SetFEPosition(int32 newPos);

	UFUNCTION(BlueprintCallable, Category = "Position")
	int32 GetFEPosition();

	AGameBoard* board;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* fireEngineMesh = nullptr;

private:
	void splashOver(ATile* targetTile, EDirection direction);
};
