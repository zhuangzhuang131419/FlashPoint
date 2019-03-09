// Fill out your copyright notice in the Description page of Project Settings.

#include "Victim.h"
#include "Engine/World.h"
#include "FPPlayerController.h"


// Sets default values
AVictim::AVictim()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	victimMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("victimMesh"));
}

void AVictim::SetIsCarried(bool carried)
{
	isCarried = carried;
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

void AVictim::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVictim, isCarried);
}

// Called when the game starts or when spawned
void AVictim::BeginPlay()
{
	Super::BeginPlay();
	// replicate the victim
	if (HasAuthority()) {
		SetReplicates(true);
	}

}

// Called every frame
void AVictim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}