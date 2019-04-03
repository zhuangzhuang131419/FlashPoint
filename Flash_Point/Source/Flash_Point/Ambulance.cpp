// Fill out your copyright notice in the Description page of Project Settings.

#include "Ambulance.h"
#include "GameBoard.h"
#include "Victim.h"
#include "UserWidget.h"
#include "MenuSystem/AmbulanceOperationsMenu.h"

// Sets default values
AAmbulance::AAmbulance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ambulanceMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ambulanceMesh"));
}

// Called when the game starts or when spawned
void AAmbulance::BeginPlay()
{
	Super::BeginPlay();

	OnClicked.AddUniqueDynamic(this, &AAmbulance::OnAmbulanceClicked);

	// Create menu list widget
	UWorld* world = GetWorld();
	if (ensure(world))
	{
		localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
		if (ensure(localPlayer))
		{
			AmbulanceOperationsUI = CreateWidget<UAmbulanceOperationsMenu>(localPlayer, AmbulanceClass);
			if (ensure(AmbulanceOperationsUI))
			{
				AmbulanceOperationsUI->AddToViewport();
				AmbulanceOperationsUI->SetVisibility(ESlateVisibility::Collapsed);
				AmbulanceOperationsUI->AssociatePlayer(localPlayer);
			}
		}
	}

	if (HasAuthority()) { SetReplicates(true); }
}

void AAmbulance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAmbulance, ambulanceMesh);
}

// Called every frame
void AAmbulance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmbulance::SetAmbulancePosition(int32 newPos)
{
	currentPosition = newPos;
}

int32 AAmbulance::GetAmbulancePosition()
{
	return currentPosition;
}

void AAmbulance::OnAmbulanceClicked(AActor * Target, FKey ButtonPressed)
{
	if (ButtonPressed != FKey("LeftMouseButton")) return;
	UE_LOG(LogTemp, Warning, TEXT("Ambulance has been clicked."));

	if (ensure(AmbulanceOperationsUI))
	{
		if (ensure(localPlayer))
		{
			float xPos;
			float yPos;
			localPlayer->GetMousePosition(xPos, yPos);
			AmbulanceOperationsUI->SetPositionInViewport(FVector2D(xPos, yPos));
			AmbulanceOperationsUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AAmbulance::RescueVictims(TArray<AVictim*>* victims, ATile* currentTile)
{
	//for (int i = 0; i < victims->Num(); i++)
	//{
	//	AVictim* tempVictim = (*victims)[i];
	//	if (!tempVictim) continue;
	//	tempVictim->Destroy();
	//	currentTile->SetPOIStatus(EPOIStatus::Empty);
	//	currentTile->GetGameBoard()->SetVictimSavedNum(
	//		currentTile->GetGameBoard()->victimSavedNum + 1
	//	);
	//	currentTile->GetGameBoard()->SetCurrentPOI(
	//		currentTile->GetGameBoard()->currentPOI - 1
	//	);
	//}
}