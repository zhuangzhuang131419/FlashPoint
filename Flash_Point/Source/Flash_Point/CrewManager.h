// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneralTypes.h"
#include "UnrealNetwork.h"
#include "CrewManager.generated.h"

class USwitchRoleUI;
class AFireFighterPawn;
class ALobbyManager;

UCLASS()
class FLASH_POINT_API ACrewManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrewManager();

	// FUNCTIONS
	// getter and setter for selected roles on board
	UFUNCTION(BlueprintCallable, Category = "Switch Role")
	TArray<ERoleType> GetSelectedRoles();
	UFUNCTION(BlueprintCallable, Category = "Switch Role")
	void AddToSelectedRoles(ERoleType inRole);
	UFUNCTION(BlueprintCallable, Category = "Switch Role")
	void SwitchRolesFromTo(ERoleType fromRole, ERoleType toRole);
	UFUNCTION(BlueprintCallable, Category = "Switch Role")
	void SelectRoleForFirefighter(AFireFighterPawn* inPawn, ERoleType toRole);
	UFUNCTION(BlueprintCallable, Category = "Switch Role")
	void ShowCrewChangeUI();
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void AssociatePlayer();
	// below functions are for interecting with the crew change UI
	UFUNCTION(BlueprintCallable, Category = "Switch Role")
	void SelectRole(ERoleType inRole);
	UFUNCTION(BlueprintCallable, Category = "Switch Role")
	void DeselectRole(ERoleType inRole);
	UFUNCTION(BlueprintCallable, Category = "Switch Role")
	void CloseCrewChangePanel();
	UFUNCTION(BlueprintCallable, Category = "Switch Role")
	void AutoSelectRole(AFireFighterPawn* inPawn);
	// getter and setter for lobby manager
	ALobbyManager* GetLobbyManager();
	void SetLobbyManager(ALobbyManager* inLobbyMan);
	// setting the crew change UI
	void SetCrewChangeUI(USwitchRoleUI* inSwitchRoleUI) { crewChangeUI = inSwitchRoleUI; }

protected:
	// REPLICATED FIELDS
	// Already selected roles in the game
	UPROPERTY(replicated, EditAnyWhere, Category = "Switch Role")
	TArray<ERoleType> selectedRoles;
	// All the roles that are not yet selected
	UPROPERTY(replicated, EditAnyWhere, Category = "Switch Role")
	TArray<ERoleType> availableRoles = 
	{ ERoleType::CAFSFirefighter, ERoleType::Driver, ERoleType::FireCaptain, ERoleType::Generalist, ERoleType::HazmatTechnician, 
		ERoleType::ImagingTechnician, ERoleType::Paramedic, ERoleType::RescueDog, ERoleType::RescueSpecialist, ERoleType::Veteran };
	// Already selected roles in the game
	UPROPERTY(replicated, EditAnyWhere, Category = "Switch Role")
	bool isInGame = false;

	// FIELDS
	class AFPPlayerController* localPlayer = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	TSubclassOf<USwitchRoleUI> SwitchRoleWidget = nullptr;
	USwitchRoleUI* crewChangeUI = nullptr;
	// if the crew manager is not in a game, this is essential
	ALobbyManager* lobbyMan = nullptr;
	
	// REPLICAION FUNCTION
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
