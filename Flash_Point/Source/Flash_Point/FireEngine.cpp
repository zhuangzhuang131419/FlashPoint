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

void AFireEngine::FireDeckGun(AFireFighterPawn* inPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("FIRING DECK GUN!"));
	// check the quadrant has firefighter or not
	int32 startx = -1;
	int32 starty = -1;
	int32 randomTargetPosition = 0;
	if (ensure(board) && ensure(board->GetFireEngineLocA()))
	{
		board->GetFireEngineLocA()->GetLocation(startx, starty);
	}
	if (startx <= 4) { startx = 1; }
	else { startx = 5; }
	if (starty <= 3) { starty = 1; }
	else { starty = 4; }
	UE_LOG(LogTemp, Warning, TEXT("x: %d, y: %d"), startx, starty);
	for (int32 x = startx; x < startx + (board->GetBoardLength() - 2) / 2 - 1; x++)
	{
		for (int32 y = starty; y < starty + (board->GetBoardWidth() - 2) / 2 - 1; y++)
		{
			if (board)
			{
				if (board->GetboardTiles()[x * board->GetBoardLength() + y]->GetPlacedFireFighters()->Num() > 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Checking tile: %d, %d"), x, y);
					UE_LOG(LogTemp, Warning, TEXT("Unable to use fire engine. Some one is in the quadrant"));
					return;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Board has not initialize."));
			}
		}
	}

	if (ensure(inPawn))
	{
		ATile* targetTile = GenerateRandomPositionInQuadrant(startx, starty);
		if (inPawn->GetFireFighterRole() == ERoleType::Driver)
		{
			AFPPlayerController* player = Cast<AFPPlayerController>(inPawn->GetController());
			if (player)
			{
				player->NotifyReRoll();
			}
		}
		if (targetTile)
		{
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
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("random Error"));
		}
		
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
}

// Called every frame
void AFireEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFireEngine::splashOver(ATile * targetTile, EDirection direction)
{
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

ATile * AFireEngine::GenerateRandomPositionInQuadrant(int32 startx, int32 starty)
{
	// randomly choose a target fire 
	int32 randx = FMath::RandRange(startx, startx + 3);
	int32 randy = FMath::RandRange(starty, starty + 2);
	UE_LOG(LogTemp, Warning, TEXT("Random Position $d, %d"), randx, randy);
	return board->GetboardTiles()[randx * 8 + randy];
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
					FireEngineOperationsUI->SetPositionInViewport(FVector2D(xPos, yPos));
					FireEngineOperationsUI->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
}
