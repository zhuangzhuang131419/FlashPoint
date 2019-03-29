// Fill out your copyright notice in the Description page of Project Settings.

#include "Ambulance.h"


// Sets default values
AAmbulance::AAmbulance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ambulanceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ambulanceMesh"));
	ambulanceMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ambulanceMeshAsset(TEXT("StaticMesh'/Game/_PersonalFolder/Cheng/AmboMesh'"));

	if (ambulanceMeshAsset.Object)
	{
		ambulanceMesh->SetStaticMesh(ambulanceMeshAsset.Object);
		ambulanceMesh->SetWorldScale3D(FVector(1.5, 1.5, 1.5));
	}
}

// Called when the game starts or when spawned
void AAmbulance::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmbulance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

