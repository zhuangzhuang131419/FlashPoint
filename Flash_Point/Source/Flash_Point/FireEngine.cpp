// Fill out your copyright notice in the Description page of Project Settings.

#include "FireEngine.h"
#include "GameBoard.h"
#include "UserWidget.h"
#include "MenuSystem/FireEngineOperationsMenu.h"

// Sets default values
AFireEngine::AFireEngine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	fireEngineMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("fireEngineMesh"));
}

void AFireEngine::FireDeckGun(AFireFighterPawn * inPawn, ATile * targetTile)
{
	if (ensure(inPawn) && ensure(targetTile))
	{
		// ensure no fighter in 
		targetTile->ShowSplashEffect();
		// extinguish fire
		targetTile->SetFireStatus(EFireStatus::Clear);
		targetTile->GetSmokeEffect()->Deactivate();
		targetTile->GetFireEffect()->Deactivate();
		// splash over
		splashOver(targetTile, EDirection::Up);
		splashOver(targetTile, EDirection::Down);
		splashOver(targetTile, EDirection::Left);
		splashOver(targetTile, EDirection::Right);
	}
}

void AFireEngine::ShowEnginePlaced(bool placed)
{
	isPlaced = placed;
	if (ensure(fireEngineMesh)) {
		fireEngineMesh->SetVisibility(placed);
	}
}

// Called when the game starts or when spawned
void AFireEngine::BeginPlay()
{
	Super::BeginPlay();

	OnClicked.AddUniqueDynamic(this, &AFireEngine::OnFireEngineClicked);

	// Create menu list widget
	UWorld* world = GetWorld();
	if (ensure(world))
	{
		localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
		if (ensure(localPlayer))
		{
			FireEngineOperationsUI = CreateWidget<UFireEngineOperationsMenu>(localPlayer, FireEngineClass);
			if (ensure(FireEngineOperationsUI))
			{
				FireEngineOperationsUI->AddToViewport();
				FireEngineOperationsUI->SetVisibility(ESlateVisibility::Collapsed);
				FireEngineOperationsUI->AssociatePlayer(localPlayer);
			}
		}
	}

	if (HasAuthority())
	{
		SetReplicates(true);
	}

}

void AFireEngine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFireEngine, fireEngineMesh);
	DOREPLIFETIME(AFireEngine, isPlaced);
	DOREPLIFETIME(AFireEngine, passengers);
	DOREPLIFETIME(AFireEngine, board);
}

void AFireEngine::Rep_FireEnginePlacing()
{
	if (ensure(fireEngineMesh)) {
		fireEngineMesh->SetVisibility(isPlaced);
	}
}

// Called every frame
void AFireEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFireEngine::splashOver(ATile * targetTile, EDirection direction)
{
	if (!ensure(targetTile)) return;
	AEdgeUnit* adjacentWall = nullptr;
	switch (direction)
	{
	case EDirection::Down:
		adjacentWall = targetTile->GetBack();
		break;
	case EDirection::Left:
		adjacentWall = targetTile->GetLeft();
		break;
	case EDirection::Right:
		adjacentWall = targetTile->GetRight();
		break;
	case EDirection::Up:
		adjacentWall = targetTile->GetFront();
		break;
	}

	if (adjacentWall)
	{
		if (!adjacentWall->IsBlocked())
		{
			ATile* adjacentTile = adjacentWall->GetOtherNeighbour(targetTile);
			if (adjacentTile)
			{
				if (adjacentTile->GetFireStatus() != EFireStatus::Clear)
				{
					UE_LOG(LogTemp, Warning, TEXT("Use fire engine at %s"), *adjacentTile->GetName());
					adjacentTile->SetFireStatus(EFireStatus::Clear);
					adjacentTile->SetFireEffect(false);
					adjacentTile->SetSmokeEffect(false);
				}
			}
		}
	}
}

void AFireEngine::OnFireEngineClicked(AActor * Target, FKey ButtonPressed)
{
	if (ButtonPressed != FKey("LeftMouseButton")) return;
	UE_LOG(LogTemp, Warning, TEXT("Fire engine has been clicked."));

	if (ensure(FireEngineOperationsUI))
	{
		if (ensure(localPlayer))
		{
			AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(localPlayer->GetPawn());
			if (ensure(fireFighterPawn))
			{
				if (fireFighterPawn->IsWithEngine())
				{
					float xPos;
					float yPos;
					localPlayer->GetMousePosition(xPos, yPos);
					if (yPos > UCarOperationsMenu::GetViewportSize().Y / 2)
					{
						yPos -= UCarOperationsMenu::GetViewportSize().Y / 4;
					}
					FireEngineOperationsUI->SetPositionInViewport(FVector2D(xPos, yPos));
					FireEngineOperationsUI->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
}

void AFireEngine::setFireEngineUI(bool status)
{
	if (ensure(FireEngineOperationsUI))
	{
		if (!status)
		{
			FireEngineOperationsUI->SetVisibility(ESlateVisibility::Collapsed);
		}
		else {
			FireEngineOperationsUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
