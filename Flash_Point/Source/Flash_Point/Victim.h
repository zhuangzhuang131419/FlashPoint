// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "UnrealNetwork.h"
#include "Victim.generated.h"

class AFPPlayerController;

UCLASS()
class FLASH_POINT_API AVictim : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVictim();

	// setter for is carried
	void SetIsCarried(bool carried);

protected:
	// REPLICATED FIELDS
	UPROPERTY(ReplicatedUsing = Rep_OnCarry, VisibleAnyWhere)
	bool isCarried = false;

	// Replication functions
	UFUNCTION()
	void Rep_OnCarry();

	// rep props
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* victimMesh = nullptr;
	
};
