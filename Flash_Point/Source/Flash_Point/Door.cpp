// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "Engine/World.h"
#include "GameBoard.h"
#include "FPPlayerController.h"
#include "FireFighterPawn.h"

ADoor::ADoor() 
{
	Door = CreateDefaultSubobject<UStaticMeshComponent>(FName("DoorMesh"));
	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(FName("DoorFrameMesh"));

	DoorFrame->AttachToComponent(WallMesh, FAttachmentTransformRules::KeepRelativeTransform, FName("DoorFrame"));
	Door->AttachToComponent(DoorFrame, FAttachmentTransformRules::KeepRelativeTransform, FName("Door"));
	
	isBlocked = true;
	edgeType = EEdgeType::Door;
}

bool ADoor::IsOpened()
{
	return opened;
}

void ADoor::SetIsOpend(bool openStatus)
{
	opened = openStatus;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	// Create binding to on cursor click
	FScriptDelegate onMouseClickedDel;
	onMouseClickedDel.BindUFunction(this, "OnDoorClicked");
	OnClicked.Add(onMouseClickedDel);
	// Create binding to on cursor over
	FScriptDelegate onMouseOverDel;
	onMouseOverDel.BindUFunction(this, "OnDoorOver");
	OnBeginCursorOver.Add(onMouseOverDel);
	// Create bidning for on cursor left
	FScriptDelegate onMouseLeftDel;
	onMouseLeftDel.BindUFunction(this, "OnDoorLeft");
	OnEndCursorOver.Add(onMouseLeftDel);
}

void ADoor::Rep_OpenStatus()
{
	if (opened) {
		OpenDoor();
	}
	else {
		CloseDoor();
	}
}

void ADoor::Rep_DoorExistence()
{
	if (isDestroyed) {
		DoorFrame->SetVisibility(false);
		Door->SetVisibility(false);
	}
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoor, DoorFrame);
	DOREPLIFETIME(ADoor, Door);
	DOREPLIFETIME(ADoor, opened);
	DOREPLIFETIME(ADoor, isDestroyed);
}

void ADoor::ChangeDoorStatus()
{
	if (opened)
	{
		opened = false;
		isBlocked = true;
		CloseDoor();
	}
	else
	{
		opened = true;
		isBlocked = false;
		OpenDoor();
	}
}

void ADoor::Damage()
{
	int32 currentHealth = gameBoard->GetCurrentGameHealth();
	currentHealth-=2;
	gameBoard->SetCurrentGameHealth(currentHealth);
	isDestroyed = true;
	isBlocked = false;
	Door->SetVisibility(false);
	DoorFrame->SetVisibility(false);
}

FEdgeSaveInfo ADoor::SaveEdge()
{
	FEdgeSaveInfo edgeInfo;
	// store all informations into the edgeInfo struct
	edgeInfo.type = EEdgeType::Door;
	edgeInfo.isBlocked = isBlocked;
	edgeInfo.chopedOrOpened = opened;
	edgeInfo.isDestroyed = isDestroyed;
	return edgeInfo;
}

void ADoor::LoadEdge(FEdgeSaveInfo edgeInfo)
{
	if (edgeInfo.chopedOrOpened) {
		OnOpenDoor();
	}
	if (edgeInfo.isDestroyed) {
		isDestroyed = true;
		isBlocked = false;
		if (ensure(DoorFrame) && ensure(Door)) {
			DoorFrame->SetVisibility(false);
			Door->SetVisibility(false);
		}
	}
}

void ADoor::OnDoorClicked(AActor* Target, FKey ButtonPressed)
{
	if (ButtonPressed != FKey("LeftMouseButton")) return;
	// cannot open or close the door if the door is destroyed already
	if (isDestroyed) return;
	UE_LOG(LogTemp, Warning, TEXT("A door has been clicked."));
	AFPPlayerController* playerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (playerController)
	{
		AFireFighterPawn* localPawn = Cast<AFireFighterPawn>(playerController->GetPawn());
		if (ensure(localPawn))
		{
			if (ensure(localPawn->GetFireFighterRole() != ERoleType::RescueDog))
			{
				if (playerController->GetCurrentOperation() == EGameOperations::OpenDoor) {
					// check if the player's firefighter has enough AP
					AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(playerController->GetPawn());
					if (ensure(fireFighterPawn)) {
						if (fireFighterPawn->GetCurrentAP() < fireFighterPawn->GetOpenConsumption()) {
							UE_LOG(LogTemp, Warning, TEXT("AP not enough to open/close door"));
							return;
						}
						if (!fireFighterPawn->IsAdjacentToWall(this)) return;
						fireFighterPawn->AdjustFireFighterAP(-fireFighterPawn->GetOpenConsumption());
					}
					if (HasAuthority()) {
						ChangeDoorStatus();
					}
					else {
						playerController->ServerOpenDoor(this);
					}
				}
			}
		}
		
	}
}

void ADoor::OnDoorOver(UPrimitiveComponent * Component)
{
	// when over, check if the operation is feasible
	AFPPlayerController* playerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!ensure(playerController))	return;
	// this will only change color if the operation is open door for local player
	// TODO for fire captain it is a little different
	if (playerController->GetCurrentOperation() == EGameOperations::OpenDoor) {
		AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(playerController->GetPawn());
		if (!ensure(fireFighterPawn))	return;
		// if firefighter is not adjacent to this wall, just return
		if (!fireFighterPawn->IsAdjacentToWall(this)) return;
		// if the firefighter's Ap is not enough, switch color to disable
		if (fireFighterPawn->GetCurrentAP() < fireFighterPawn->GetOpenConsumption()) {
			if (ensure(WallMesh)) {
				WallMesh->SetMaterial(0, unableMat);
			}
		}
		else {
			if (ensure(WallMesh)) {
				if (isDestroyed) {
					WallMesh->SetMaterial(0, unableMat);
				}
				else {
					WallMesh->SetMaterial(0, ableMat);
				}
			}
		}
	}
}

void ADoor::OnDoorLeft(UPrimitiveComponent * Component)
{
	if (ensure(WallMesh)) {
		WallMesh->SetMaterial(0, baseMat);
	}
}

