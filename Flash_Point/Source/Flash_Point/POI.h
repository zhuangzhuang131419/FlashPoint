// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealNetwork.h"
#include "POI.generated.h"

UCLASS()
class FLASH_POINT_API APOI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APOI();

	// FIELD
	UPROPERTY(replicated, VisibleAnyWhere)
	bool isAlarm;

protected:

	// replicate all needed properties
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
