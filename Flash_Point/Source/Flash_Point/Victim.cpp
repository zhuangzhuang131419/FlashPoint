// Fill out your copyright notice in the Description page of Project Settings.

#include "Victim.h"
#include "Engine/World.h"
#include "FPPlayerController.h"


// Sets default values
AVictim::AVictim()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	victimMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("victimMesh"));
}

void AVictim::SetVictimLoc(FVector loc)
{
	victimLoc = loc;
}

void AVictim::Rep_OnCarry()
{
	if (isCarried) {
		victimMesh->SetVisibility(false);
	}
	else {
		victimMesh->SetVisibility(true);
	}
}

void AVictim::Rep_OnVictimLocationChanged()
{
	// when the actor's world location changed, reset it
	SetActorLocation(victimLoc);
}

void AVictim::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVictim, isCarried);
	DOREPLIFETIME(AVictim, victimLoc);
}

// Called when the game starts or when spawned
void AVictim::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		SetReplicates(true);
	}
}

// Called every frame
void AVictim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}