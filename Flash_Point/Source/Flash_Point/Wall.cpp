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


void AWall::Damage()
{
	Super::Damage();
	int32 currentHealth = gameBoard->GetCurrentGameHealth();
	currentHealth--;
	gameBoard->SetCurrentGameHealth(currentHealth);
}

FEdgeSaveInfo AWall::SaveEdge()
{
	FEdgeSaveInfo edgeInfo;
	// Save all values with the wall into the struct
	edgeInfo.type = EEdgeType::Wall;
	edgeInfo.isBlocked = isBlocked;
	edgeInfo.chopedOrOpened = isChoped;
	edgeInfo.isDestroyed = false;
	return edgeInfo;
}

void AWall::LoadEdge(FEdgeSaveInfo edgeInfo)
{
	if (edgeInfo.chopedOrOpened) {
		ChopWall();
	}
	if (!edgeInfo.isBlocked) {
		isBlocked = false;
		if (ensure(WallMesh)) {
			WallMesh->SetVisibility(false);
		}
	}
}


void AWall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWall, isChoped);
}

void AWall::Rep_BlockStatus()
{
	if (!isBlocked) {
		if (ensure(WallMesh)) {
			WallMesh->SetVisibility(false);
		}
	}
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
	if (!isBlocked) return;
	AFPPlayerController* playerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (playerController)
	{
		if (playerController->GetCurrentOperation() == EGameOperations::ChopWall) {	
			AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(playerController->GetPawn());
			if (ensure(fireFighterPawn)) {
				if (ensure(fireFighterPawn->GetFireFighterRole() != ERoleType::RescueDog))
				{
					if (fireFighterPawn->GetCurrentAP() < fireFighterPawn->GetChopConsumption()) {
						UE_LOG(LogTemp, Warning, TEXT("AP not enough to chop wall"));
						return;
					}
					if (!fireFighterPawn->IsAdjacentToWall(this)) return;
					fireFighterPawn->AdjustFireFighterAP(-fireFighterPawn->GetChopConsumption());
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
	}
}

void AWall::OnWallOver(UPrimitiveComponent * Component)
{
	// if the wall is destroyed, just return
	if (!isBlocked)	return;
	AFPPlayerController* playerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!ensure(playerController))	return;
	// this will only change color if the operation is chop wall for local player
	// TODO for fire captain it is a little different
	if (playerController->GetCurrentOperation() == EGameOperations::ChopWall) {
		AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(playerController->GetPawn());
		if (!ensure(fireFighterPawn))	return;
		// if firefighter is not adjacent to this wall, just return
		if (!fireFighterPawn->IsAdjacentToWall(this)) return;
		// if the firefighter's Ap is not enough, switch color to disable
		if (fireFighterPawn->GetCurrentAP() < fireFighterPawn->GetChopConsumption()) {
			if (ensure(WallMesh)) {
				WallMesh->SetMaterial(0, unableMat);
			}
		}
		else {
			if (ensure(WallMesh)) {
				WallMesh->SetMaterial(0, ableMat);
			}
		}
	}
}

void AWall::OnWallLeft(UPrimitiveComponent * Component)
{
	if (ensure(WallMesh)) {
		WallMesh->SetMaterial(0, baseMat);
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
	// Create binding to on cursor over
	FScriptDelegate onMouseOverDel;
	onMouseOverDel.BindUFunction(this, "OnWallOver");
	OnBeginCursorOver.Add(onMouseOverDel);
	// Create bidning for on cursor left
	FScriptDelegate onMouseLeftDel;
	onMouseLeftDel.BindUFunction(this, "OnWallLeft");
	OnEndCursorOver.Add(onMouseLeftDel);
}