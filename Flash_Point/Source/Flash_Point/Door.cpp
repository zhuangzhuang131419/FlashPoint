// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "Engine/World.h"
#include "FPPlayerController.h"

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
	Super::Damage();
	Door->SetVisibility(false);
	DoorFrame->SetVisibility(false);
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
		if (playerController->GetCurrentOperation() == EGameOperations::OpenDoor) {
			if (HasAuthority()) {
				ChangeDoorStatus();
			}
			else {
				playerController->ServerOpenDoor(this);
			}
		}
	}
}

