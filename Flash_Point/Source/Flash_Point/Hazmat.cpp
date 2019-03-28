// Fill out your copyright notice in the Description page of Project Settings.

#include "Hazmat.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AHazmat::AHazmat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	hazmatMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("HazmatMesh"));
}

void AHazmat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHazmat, isCarried);
	DOREPLIFETIME(AHazmat, hazmatLoc);
}

// Called when the game starts or when spawned
void AHazmat::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority()) {
		SetReplicates(true);
	}
}

// Called every frame
void AHazmat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHazmat::SetHazmatLoc(FVector loc)
{
	hazmatLoc = loc;
}

void AHazmat::Rep_OnCarryHaz()
{
	if (isCarried) {
		hazmatMesh->SetVisibility(false);
	}
	else {
		hazmatMesh->SetVisibility(true);
	}
}

void AHazmat::Rep_OnHazmatLocChanged()
{
	SetActorLocation(hazmatLoc);
}

