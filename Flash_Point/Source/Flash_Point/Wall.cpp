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

	// Create binding to on cursor click
	FScriptDelegate onMouseClickedDel;
	onMouseClickedDel.BindUFunction(this, "OnCursorClicked");
	OnClicked.Add(onMouseClickedDel);

	SetReplicates(true);
	SetReplicateMovement(true);
}

void AWall::OnCursorClicked(UPrimitiveComponent* Component) {
	UE_LOG(LogTemp, Warning, TEXT("Mouse Clicked"));
	UE_LOG(LogTemp, Warning, TEXT("Chop Wall has been selected."));
	AFPPlayerController* playerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (playerController)
	{
		if (playerController->GetCurrentOperation() == EGameOperations::ChopWall) {
			
			if (isChoped)
			{
				WallMesh->DestroyComponent();
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
	}

}



