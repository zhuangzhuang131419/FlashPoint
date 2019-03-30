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

void ACrewManager::ShowCrewChangeUI()
{
	if (ensure(crewChangeUI)) {
		// update the UI corresponding to selected roles first
		crewChangeUI->UpdateAvailableRoles(selectedRoles);
		crewChangeUI->SetVisibility(ESlateVisibility::Visible);
	}
	// if this is an ingame crew manager, disable player operations for crew changing
	if (isInGame) {
		if (ensure(localPlayer)) {
			localPlayer->EnableOperations(false);
		}
	}
}

void ACrewManager::AssociatePlayer()
{
	UWorld* world = GetWorld();
	if (!ensure(world)) return;
	localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
	// if the UI is already created, do not create it again
	if (!ensure(localPlayer)) return;
	if (ensure(localPlayer && ensure(SwitchRoleWidget))) {
		crewChangeUI = CreateWidget<USwitchRoleUI>(localPlayer, SwitchRoleWidget);
		if (ensure(crewChangeUI)) {
			crewChangeUI->BindCrewManger(this);
			crewChangeUI->AddToViewport();
			crewChangeUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ACrewManager::SelectRole(ERoleType inRole)
{
	// do operations only if the role is not seleted yet
	if (!selectedRoles.Contains(inRole)) {
		// if the crew manager is in game, do role switch
		if (isInGame) {
			if (ensure(localPlayer)) {
				localPlayer->SwitchRole(inRole);
			}
		}
	}

	// close the role selection widget after done role switching
	if (ensure(crewChangeUI)) {
		crewChangeUI->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ACrewManager::CloseCrewChangePanel()
{
	// Collapse the UI
	if (ensure(crewChangeUI)) {
		crewChangeUI->SetVisibility(ESlateVisibility::Collapsed);
	}
	// if the crew manager is a ingame crew manager, also enable player operations
	if (isInGame) {
		if (ensure(localPlayer)) {
			localPlayer->EnableOperations(true);
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
