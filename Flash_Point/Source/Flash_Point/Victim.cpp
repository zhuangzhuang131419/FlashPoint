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
	// Create binding to on cursor click
	FScriptDelegate onMouseClickedDel;
	onMouseClickedDel.BindUFunction(this, "OnTileClicked");
	OnClicked.Add(onMouseClickedDel);
}

void AVictim::OnTileClicked(AActor * Target, FKey ButtonPressed)
{
	if (ButtonPressed != FKey("LeftMouseButton")) return;
	UE_LOG(LogTemp, Warning, TEXT("Victim is been clicked"));
	AFPPlayerController* playerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (ensure(playerController))
	{
		if (playerController->GetCurrentOperation() == EGameOperations::Carry)
		{
			if (playerController->GetCarriedVictim() == nullptr)
			{
				APawn* controlledPawn = Cast<APawn>(playerController);
				// if (this->GetActorLocation() == controlledPawn->GetActorLocation())
				{
					playerController->SetCarriedVictim(this);
					victimMesh->SetVisibility(false);
				}
			}
		}
	}
}

// Called every frame
void AVictim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}