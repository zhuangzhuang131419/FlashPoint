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

// Called when the game starts or when spawned
void AVictim::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVictim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}