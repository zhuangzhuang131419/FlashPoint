// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralTypes.h"
#include "GameFramework/Actor.h"
#include "Ambulance.generated.h"

class AGameBoard;
class ATile;
class AVictim;
class UAmbulanceOperationsMenu;
class AFPPlayerController;
class AFireFighterPawn;

UCLASS()
class FLASH_POINT_API AAmbulance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmbulance();

	TArray<AFireFighterPawn*>* GetPassengers() { return &passengers; }

	UPROPERTY(BlueprintReadWrite, Category = "Position Info")
	int32 currentPosition = -1;

	UFUNCTION(BlueprintCallable, Category = "Position")
	void SetAmbulancePosition(int32 newPos);

	UFUNCTION(BlueprintCallable, Category = "Position")
	int32 GetAmbulancePosition();

	// Getter for playing board
	AGameBoard* GetPlayingBoard() { return board; }
	void SetPlayingGameBoard(AGameBoard* current) { board = current; }
	void setAmbulanceUI(bool status);
	UFUNCTION()
	void OnAmbulanceClicked(AActor* Target, FKey ButtonPressed);
	void ShowAmbulancePlaced(bool placed);
	void RescueVictims();
	void RescueVictims(ATile* targetTile);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	// UI related fields
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	UAmbulanceOperationsMenu* AmbulanceOperationsUI = nullptr;

	// Field
	UPROPERTY(BlueprintReadWrite, Category = "Widget class")
	TSubclassOf<UAmbulanceOperationsMenu> AmbulanceClass = nullptr;

	AFPPlayerController* localPlayer = nullptr;
	UPROPERTY(replicated, VisibleAnyWhere, Category = "Widget class")
	TArray<AFireFighterPawn*> passengers;

	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadWrite)
	UStaticMeshComponent* ambulanceMesh = nullptr;

	UPROPERTY(ReplicatedUsing = Rep_AmbulancePlacing, VisibleAnyWhere, BlueprintReadWrite)
	bool isPlaced = true;

	AGameBoard* board;
	

	UFUNCTION()
	void Rep_AmbulancePlacing();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
