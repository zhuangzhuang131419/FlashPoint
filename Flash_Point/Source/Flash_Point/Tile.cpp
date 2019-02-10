// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Intialize the floor and plane objects
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Tile Mesh"));
	ColorPlane = CreateDefaultSubobject<UStaticMeshComponent>(FName("Color Plane"));
	ColorPlane->AttachToComponent(TileMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName("Plane"));

}

// Here is the function to bind all input bindings
void ATile::BindCursorFunc()
{
	// Create binding to on cursor over
	FScriptDelegate onMouseOverDel;
	onMouseOverDel.BindUFunction(this, "OnCursorOver");
	OnBeginCursorOver.Add(onMouseOverDel);

	// Create binding to on cursor click
	FScriptDelegate onMouseClickedDel;
	onMouseClickedDel.BindUFunction(this, "OnCursorClicked");
	OnClicked.Add(onMouseClickedDel);

	// Create binding to on cursor leave
	FScriptDelegate onMouseLeftDel;
	onMouseLeftDel.BindUFunction(this, "OnCursorLeft");
	OnEndCursorOver.Add(onMouseLeftDel);
}

void ATile::OnCursorOver(UPrimitiveComponent * Component)
{
	PlaneColorSwitch(ableMat);
	UE_LOG(LogTemp, Warning, TEXT("Mouse Over"));
}

void ATile::OnCursorClicked(UPrimitiveComponent* Component)
{
	UE_LOG(LogTemp, Warning, TEXT("Mouse Clicked"));
}

void ATile::OnCursorLeft(UPrimitiveComponent * Component)
{
	PlaneColorSwitch(baseMat);
	UE_LOG(LogTemp, Warning, TEXT("Mouse Left"));
}

void ATile::PlaneColorSwitch(UMaterialInterface * mat)
{
	if (ensure(ColorPlane)) {
		ColorPlane->SetMaterial(0, mat);
	}
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
	BindCursorFunc();

	GetWorld()->SpawnActor<AEdgeUnit>(
		EdgeClass,
		TileMesh->GetSocketLocation(FName("WallLeft")),
		TileMesh->GetSocketRotation(FName("WallLeft"))
	);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

