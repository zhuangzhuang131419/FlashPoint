// Fill out your copyright notice in the Description page of Project Settings.

#include "Citizen.h"


// Sets default values
ACitizen::ACitizen()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACitizen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACitizen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACitizen::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACitizen::AttachCitizenToFireFighter()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Added"));
	// Find the local player and local pawn
	AFPPlayerController* LocalPlayer = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
	AFireFighterPawn* Player = Cast<AFireFighterPawn>(LocalPlayer->GetPawn());
	
	if (Player->GetVictim() == nullptr)
	{
		Player->CarrayVictim(this);
	}
}

