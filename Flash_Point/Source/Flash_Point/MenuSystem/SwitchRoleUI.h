// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SwitchRoleUI.generated.h"

class UButton;
class ACrewManager;
enum class ERoleType : uint8;

/**
 * 
 */
UCLASS()
class FLASH_POINT_API USwitchRoleUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCrewManger(ACrewManager* inCrewMan) { crewMan = inCrewMan; }
	// function for updating selectable roles
	void UpdateAvailableRoles(TArray<ERoleType> inRoles);
	
protected:
	// BINDED WIDGETS
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* CloseSelectionPanel = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* SelParamedic = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* SelImageTec = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* SelCAFS = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* SelHazmatTec = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* SelVeteran = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* SelRescueDog = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* SelFireCaptain = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* SelGeneralist = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* SelRescueSpec = nullptr;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* SelOperator = nullptr;

	// Fields
	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	ACrewManager* crewMan;

	// FUNCTIONS
	virtual bool Initialize() override;
	// functions binded to buttons
	UFUNCTION()
	void OnCloseCrewChangePanel();
	UFUNCTION()
	void OnSelParamedic();
	UFUNCTION()
	void OnSelImageTec();
	UFUNCTION()
	void OnSelCAFS();
	UFUNCTION()
	void OnSelHazmatTec();
	UFUNCTION()
	void OnSelVeteran();
	UFUNCTION()
	void OnSelRescueDog();
	UFUNCTION()
	void OnSelFireCaptain();
	UFUNCTION()
	void OnSelGeneralist();
	UFUNCTION()
	void OnSelRescueSpec();
	UFUNCTION()
	void OnSelOperator();
	
	
};
