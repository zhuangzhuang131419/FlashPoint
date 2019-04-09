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
	DOREPLIFETIME(AAmbulance, isPlaced);
	DOREPLIFETIME(AAmbulance, passengers);
	DOREPLIFETIME(AAmbulance, board);
}

void AAmbulance::Rep_AmbulancePlacing()
{
	if (ensure(ambulanceMesh)) {
		ambulanceMesh->SetVisibility(isPlaced);
	}
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
			AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(localPlayer->GetPawn());
			if (ensure(fireFighterPawn))
			{
				// if (fireFighterPawn->IsWithAmbulance())
				{
					float xPos;
					float yPos;
					localPlayer->GetMousePosition(xPos, yPos);

					if (yPos > UCarOperationsMenu::GetViewportSize().Y / 2) {
						yPos -= UCarOperationsMenu::GetViewportSize().Y / 4;
					}
					AmbulanceOperationsUI->SetPositionInViewport(FVector2D(xPos, yPos));
					AmbulanceOperationsUI->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
}

void AAmbulance::ShowAmbulancePlaced(bool placed)
{
	isPlaced = placed;
	if (ensure(ambulanceMesh)) {
		ambulanceMesh->SetVisibility(placed);
	}
}

void AAmbulance::RescueVictims()
{
	UE_LOG(LogTemp, Warning, TEXT("Ambulance rescue victim."));
	if (ensure(board) && ensure(board->GetAmbulanceLocA()) && ensure(board->GetAmbulanceLocB()))
	{
		RescueVictims(board->GetAmbulanceLocA());
		RescueVictims(board->GetAmbulanceLocB());
	}
}

void AAmbulance::RescueVictims(ATile* targetTile)
{
	if (ensure(targetTile) && ensure(targetTile->GetType() == ETileType::AmbulancePark))
	{
		for (size_t i = 0; i < targetTile->GetVictims()->Num(); i++)
		{
			AVictim* tempVictim = (*targetTile->GetVictims())[i];
			if (ensure(tempVictim) && ensure(targetTile->GetGameBoard()))
			{
				tempVictim->Destroy();
				targetTile->SetPOIStatus(EPOIStatus::Empty);
				targetTile->GetGameBoard()->SetVictimSavedNum(
					targetTile->GetGameBoard()->victimSavedNum + 1
				);
				UE_LOG(LogTemp, Warning, TEXT("Current saved victim: %d"), targetTile->GetGameBoard()->victimSavedNum);
				targetTile->GetGameBoard()->SetCurrentPOI(
					targetTile->GetGameBoard()->currentPOI - 1
				);
				UE_LOG(LogTemp, Warning, TEXT("Current saved victim: %d"), targetTile->GetGameBoard()->currentPOI);
			}
		}
		if (ensure(targetTile->GetVictims())) {
			targetTile->GetVictims()->Empty();
		}
	}
}

void AAmbulance::setAmbulanceUI(bool status)
{
	if (ensure(AmbulanceOperationsUI))
	{
		if (!status)
		{
			AmbulanceOperationsUI->SetVisibility(ESlateVisibility::Collapsed);
		}
		else {
			AmbulanceOperationsUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}