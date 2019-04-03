// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralTypes.h"
#include "GameFramework/Actor.h"
#include "FireEngine.generated.h"

class ATile;
class AGameBoard;
class UFireEngineOperationsMenu;
class AFPPlayerController;
class AFireFighterPawn;

UCLASS()
class FLASH_POINT_API AFireEngine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireEngine();

	void FireDeckGun();

	UPROPERTY(BlueprintReadWrite, Category = "Position Info")
	int32 currentPosition = -1;

	UFUNCTION(BlueprintCallable, Category = "Position")
	void SetFEPosition(int32 newPos);

	UFUNCTION(BlueprintCallable, Category = "Position")
	int32 GetFEPosition();

	UFUNCTION()
	void OnFireEngineClicked(AActor* Target, FKey ButtonPressed);

	// Getter for playing board
	AGameBoard* GetPlayingBoard() { return board; }
	void SetPlayingGameBoard(AGameBoard* current) { board = current; }

	TArray<AFireFighterPawn*>* GetPassengers() { return &passengers; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	// UI related fields
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	UFireEngineOperationsMenu* FireEngineOperationsUI = nullptr;

	// Field
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UFireEngineOperationsMenu> FireEngineClass = nullptr;

	AFPPlayerController* localPlayer = nullptr;
	UPROPERTY(VisibleAnyWhere, Category = "Widget class")
	TArray<AFireFighterPawn*> passengers;

	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadWrite)
	UStaticMeshComponent* fireEngineMesh = nullptr;

	AGameBoard* board;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void splashOver(ATile* targetTile, EDirection direction);
};
