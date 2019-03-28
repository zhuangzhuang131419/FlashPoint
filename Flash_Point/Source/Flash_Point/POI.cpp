// Fill out your copyright notice in the Description page of Project Settings.

#include "POI.h"


// Sets default values
APOI::APOI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void APOI::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APOI, isAlarm);
}

// Called when the game starts or when spawned
void APOI::BeginPlay()
{
	Super::BeginPlay();

	// replicate this item
	if (HasAuthority()) {
		SetReplicates(true);
	}
	
}

// Called every frame
void APOI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

