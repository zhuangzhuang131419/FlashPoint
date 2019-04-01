// Fill out your copyright notice in the Description page of Project Settings.

#include "Victim.h"
#include "Engine/World.h"
#include "FPPlayerController.h"
#include "FireFighterPawn.h"
#include "Tile.h"


// Sets default values
AVictim::AVictim()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	victimMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("victimMesh"));
}

void AVictim::Rep_OnCarry()
{
	if (isCarried) {
		UE_LOG(LogTemp, Warning, TEXT("Set to invisible"));
		victimMesh->SetVisibility(false);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Set to visible"));
		victimMesh->SetVisibility(true);
	}
}

void AVictim::Rep_OnVictimLocationChanged()
{
	// when the actor's world location changed, reset it
	SetActorLocation(victimLoc);
}

void AVictim::OnVictimClicked(AActor * Target, FKey ButtonPressed)
{
	if (ButtonPressed != FKey("LeftMouseButton")) return;
	UE_LOG(LogTemp, Warning, TEXT("Victim has been clicked"));
	/*
	AFPPlayerController* playerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
	AFireFighterPawn* fireFireterPawn = Cast<AFireFighterPawn>(playerController->GetPawn());
	if (ensure(fireFireterPawn))
	{
		fireFireterPawn->SetLeading(this);
		this->SetActorLocation(fireFireterPawn->GetActorLocation() - FVector(0, 100, 0));
		UE_LOG(LogTemp, Warning, TEXT("Victim has been lead"));
	}
	*/
}

void AVictim::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVictim, isCarried);
	DOREPLIFETIME(AVictim, victimLoc);
	DOREPLIFETIME(AVictim, isHealed);
	//DOREPLIFETIME(AVictim, placedOn);
}

// Called when the game starts or when spawned
void AVictim::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		SetReplicates(true);
	}

	FScriptDelegate onMouseClickedDel;
	onMouseClickedDel.BindUFunction(this, "OnVictimClicked");
	OnClicked.Add(onMouseClickedDel);
}

// Called every frame
void AVictim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}