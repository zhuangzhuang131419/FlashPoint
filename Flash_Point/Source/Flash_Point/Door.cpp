// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"

ADoor::ADoor() 
{
	Door = CreateDefaultSubobject<UStaticMeshComponent>(FName("DoorMesh"));
	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(FName("DoorFrameMesh"));

	DoorFrame->AttachToComponent(WallMesh, FAttachmentTransformRules::KeepRelativeTransform, FName("DoorFrame"));
	Door->AttachToComponent(DoorFrame, FAttachmentTransformRules::KeepRelativeTransform, FName("Door"));
	
	// isBlocked = true;
	edgeType = EEdgeType::Door;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

