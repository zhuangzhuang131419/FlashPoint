// Fill out your copyright notice in the Description page of Project Settings.

#include "Wall.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "GameBoard.h"
#include "FPPlayerController.h"


AWall::AWall() {
	isBlocked = true;
	edgeType = EEdgeType::Wall;
	isChoped = false;
}

void AWall::ChopWall()
{
	if (isChoped)
	{
		WallMesh->SetVisibility(false);
		isBlocked = false;
	}
	else {
		isChoped = true;
		FVector newRotation3D = FVector(1.0f, 1.0f, 0.5f);
		WallMesh->SetRelativeScale3D(newRotation3D);
	}
	int32 currentHealth = gameBoard->GetCurrentGameHealth();
	currentHealth--;
	gameBoard->SetCurrentGameHealth(currentHealth);
}

void AWall::UpdateWallMesh(bool chopedDown)
{
	if (chopedDown) {
		WallMesh->SetVisibility(false);
	}
	else{
		FVector newRotation3D = FVector(1.0f, 1.0f, 0.5f);
		WallMesh->SetRelativeScale3D(newRotation3D);
	}
}

void AWall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWall, isChoped);
}

void AWall::Rep_WallMesh()
{
	if (isChoped) {
		FVector newRotation3D = FVector(1.0f, 1.0f, 0.5f);
		WallMesh->SetRelativeScale3D(newRotation3D);
	}
}

void AWall::OnWallClicked(AActor* Target, FKey ButtonPressed) {
	if (ButtonPressed != FKey("LeftMouseButton")) return;
	UE_LOG(LogTemp, Warning, TEXT("Wall Clicked"));
	AFPPlayerController* playerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (playerController)
	{
		if (playerController->GetCurrentOperation() == EGameOperations::ChopWall) {		
			if (HasAuthority()) {
				ChopWall();
				//playerController->ClientWallMeshUpdate(this, !isBlocked);
			}
			else {
				playerController->ServerChopWall(this);
			}
		}
	}
}

void AWall::BeginPlay()
{
	if (HasAuthority()) {
		SetReplicates(true);
	}
	Super::BeginPlay();
	// Create binding to on cursor click
	FScriptDelegate onMouseClickedDel;
	onMouseClickedDel.BindUFunction(this, "OnWallClicked");
	OnClicked.Add(onMouseClickedDel);
}



