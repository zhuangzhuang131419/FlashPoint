// Fill out your copyright notice in the Description page of Project Settings.

#include "CrewManager.h"
#include "MenuSystem/SwitchRoleUI.h"
#include "FPPlayerController.h"


// Sets default values
ACrewManager::ACrewManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}


TArray<ERoleType> ACrewManager::GetSelectedRoles()
{
	return selectedRoles;
}

void ACrewManager::AddToSelectedRoles(ERoleType inRole)
{
	selectedRoles.Add(inRole);
}

void ACrewManager::SwitchRolesFromTo(ERoleType fromRole, ERoleType toRole)
{
	selectedRoles.Remove(fromRole);
	selectedRoles.Add(toRole);
}

void ACrewManager::AssociatePlayer(AFPPlayerController * inPlayer)
{
	localPlayer = inPlayer;
	if (ensure(localPlayer && ensure(SwitchRoleWidget))) {
		crewChangeUI = CreateWidget<USwitchRoleUI>(localPlayer, SwitchRoleWidget);
		if (ensure(crewChangeUI)) {
			crewChangeUI->AddToViewport();
		}
	}
}

void ACrewManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACrewManager, selectedRoles);
	DOREPLIFETIME(ACrewManager, isInGame);
}

// Called when the game starts or when spawned
void ACrewManager::BeginPlay()
{
	// replicate this object if is on server
	if (HasAuthority()) {
		SetReplicates(true);
	}

	Super::BeginPlay();
	
}

// Called every frame
void ACrewManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

