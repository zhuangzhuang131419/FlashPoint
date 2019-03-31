// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "UnrealNetwork.h"
#include "Victim.generated.h"

class AFPPlayerController;
class ATile;

UCLASS()
class FLASH_POINT_API AVictim : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVictim();

	// setter for is carried
	void SetIsCarried(bool carried) { isCarried = carried; }
	// Getter and setter for is healed
	bool IsHealed() { return isHealed; }
	void SetIsHealed(bool healed) { isHealed = healed; }
	// setter for the actor's new world location
	void SetVictimLoc(FVector loc) { victimLoc = loc; }

	// Getter and setter for placedOn
	ATile* GetPlacedOn() { return placedOn; }
	void SetPlacedOn(ATile* current) { placedOn = current; }

protected:
	// REPLICATED FIELDS
	UPROPERTY(ReplicatedUsing = Rep_OnCarry, VisibleAnyWhere)
	bool isCarried = false;
	UPROPERTY(ReplicatedUsing = Rep_OnCarry, VisibleAnyWhere)
	bool isHealed = false;
	UPROPERTY(ReplicatedUsing = Rep_OnVictimLocationChanged, VisibleAnyWhere)
	FVector victimLoc;

	ATile* placedOn = nullptr;

	// Replication functions
	UFUNCTION()
	void Rep_OnCarry();
	UFUNCTION()
	void Rep_OnVictimLocationChanged();

	// Cursor clicked method
	UFUNCTION()
	void OnVictimClicked(AActor* Target, FKey ButtonPressed);

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
