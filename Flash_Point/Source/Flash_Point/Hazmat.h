// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneralTypes.h"
#include "UnrealNetwork.h"
#include "Hazmat.generated.h"

UCLASS()
class FLASH_POINT_API AHazmat : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHazmat();

	UPROPERTY(ReplicatedUsing = Rep_OnCarryHaz, VisibleAnyWhere)
	bool isCarried;
	UPROPERTY(ReplicatedUsing = Rep_OnHazmatLocChanged, VisibleAnyWhere)
	FVector hazmatLoc;

	// setter for the actor's new world location
	void SetHazmatLoc(FVector loc);

protected:
	// Replication functions
	UFUNCTION()
	void Rep_OnCarryHaz();
	UFUNCTION()
	void Rep_OnHazmatLocChanged();

	// rep props
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* hazmatMesh = nullptr;
	
};
