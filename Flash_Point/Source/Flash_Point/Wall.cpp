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

void AWall::ServerChop_Implementation()
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

bool AWall::ServerChop_Validate()
{
	return true;
}

void AWall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWall, isChoped);
}

void AWall::OnCursorClicked(UPrimitiveComponent* Component) {
	UE_LOG(LogTemp, Warning, TEXT("Mouse Clicked"));
	AFPPlayerController* playerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (playerController)
	{
		if (playerController->GetCurrentOperation() == EGameOperations::ChopWall) {
			ServerChop();
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
	onMouseClickedDel.BindUFunction(this, "OnCursorClicked");
	OnClicked.Add(onMouseClickedDel);
}



