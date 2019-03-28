// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdgeUnit.h"
#include "Components/StaticMeshComponent.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API ADoor : public AEdgeUnit
{
	GENERATED_BODY()
	
public:
	ADoor();

	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadWrite)
	UStaticMeshComponent* Door = nullptr;

	UPROPERTY(replicated, VisibleAnyWhere, BlueprintReadWrite)
	UStaticMeshComponent* DoorFrame = nullptr;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CloseDoor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OpenDoor();

	UFUNCTION(BlueprintCallable)
	bool IsOpened();

	UFUNCTION(BlueprintCallable)
	void SetIsOpend(bool openStatus);

	void ChangeDoorStatus();

	virtual void Damage() override;
	virtual FEdgeSaveInfo SaveEdge() override;
	virtual void LoadEdge(FEdgeSaveInfo edgeInfo) override;
	

protected:
	virtual void BeginPlay() override;

	// properties to be replicated
	UPROPERTY(ReplicatedUsing=Rep_OpenStatus, VisibleAnyWhere, BlueprintReadWrite)
	bool opened = false;
	UPROPERTY(ReplicatedUsing = Rep_DoorExistence, VisibleAnyWhere, BlueprintReadWrite)
	bool isDestroyed = false;

	// functions to replicate specified fields
	// replicate openning of the door
	UFUNCTION()
	void Rep_OpenStatus();
	UFUNCTION()
	void Rep_DoorExistence();
	// mark all replicated properties
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	UFUNCTION()
	void OnDoorClicked(AActor * Target, FKey ButtonPressed);


private:

	

};
