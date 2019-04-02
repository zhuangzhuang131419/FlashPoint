// Fill out your copyright notice in the Description page of Project Settings.

#include "SwitchRoleUI.h"
#include "Components/Button.h"
#include "CrewManager.h"

bool USwitchRoleUI::Initialize() {
	bool success = Super::Initialize();
	// Bind functions to buttons
	if (success) {
		if (!ensure(CloseSelectionPanel)) return false;
		CloseSelectionPanel->OnClicked.AddDynamic(this, &USwitchRoleUI::OnCloseCrewChangePanel);
		if (!ensure(SelParamedic)) return false;
		SelParamedic->OnClicked.AddDynamic(this, &USwitchRoleUI::OnSelParamedic);
		if (!ensure(SelImageTec)) return false;
		SelImageTec->OnClicked.AddDynamic(this, &USwitchRoleUI::OnSelImageTec);
		if (!ensure(SelCAFS)) return false;
		SelCAFS->OnClicked.AddDynamic(this, &USwitchRoleUI::OnSelCAFS);
		if (!ensure(SelHazmatTec)) return false;
		SelHazmatTec->OnClicked.AddDynamic(this, &USwitchRoleUI::OnSelHazmatTec);
		if (!ensure(SelVeteran)) return false;
		SelVeteran->OnClicked.AddDynamic(this, &USwitchRoleUI::OnSelVeteran);
		if (!ensure(SelRescueDog)) return false;
		SelRescueDog->OnClicked.AddDynamic(this, &USwitchRoleUI::OnSelRescueDog);
		if (!ensure(SelFireCaptain)) return false;
		SelFireCaptain->OnClicked.AddDynamic(this, &USwitchRoleUI::OnSelFireCaptain);
		if (!ensure(SelGeneralist)) return false;
		SelGeneralist->OnClicked.AddDynamic(this, &USwitchRoleUI::OnSelGeneralist);
		if (!ensure(SelRescueSpec)) return false;
		SelRescueSpec->OnClicked.AddDynamic(this, &USwitchRoleUI::OnSelRescueSpec);
		if (!ensure(SelOperator)) return false;
		SelOperator->OnClicked.AddDynamic(this, &USwitchRoleUI::OnSelOperator);
	}
	return success;
}

void USwitchRoleUI::UpdateAvailableRoles(TArray<ERoleType> inRoles) {
	UE_LOG(LogTemp, Warning, TEXT("Refreshing role selecting options"));
	// ensure buttons to preotect nullptr
	if (!ensure(SelParamedic)) return;
	if (!ensure(SelImageTec)) return;
	if (!ensure(SelCAFS)) return;
	if (!ensure(SelHazmatTec)) return;
	if (!ensure(SelVeteran)) return;
	if (!ensure(SelRescueDog)) return;
	if (!ensure(SelFireCaptain)) return;
	if (!ensure(SelGeneralist)) return;
	if (!ensure(SelRescueSpec)) return;
	if (!ensure(SelOperator)) return;
	// enable all buttons
	SelParamedic->SetIsEnabled(true);
	SelImageTec->SetIsEnabled(true);
	SelCAFS->SetIsEnabled(true);
	SelHazmatTec->SetIsEnabled(true);
	SelVeteran->SetIsEnabled(true);
	SelRescueDog->SetIsEnabled(true);
	SelFireCaptain->SetIsEnabled(true);
	SelGeneralist->SetIsEnabled(true);
	SelRescueSpec->SetIsEnabled(true);
	SelOperator->SetIsEnabled(true);
	// for those that are already selected, set button to false
	for (ERoleType tempRole : inRoles) {
		switch (tempRole)
		{
		case ERoleType::Basic:
			break;
		case ERoleType::Paramedic:
			SelParamedic->SetIsEnabled(false);
			break;
		case ERoleType::FireCaptain:
			SelFireCaptain->SetIsEnabled(false);
			break;
		case ERoleType::ImagingTechnician:
			SelImageTec->SetIsEnabled(false);
			break;
		case ERoleType::CAFSFirefighter:
			SelCAFS->SetIsEnabled(false);
			break;
		case ERoleType::HazmatTechnician:
			SelHazmatTec->SetIsEnabled(false);
			break;
		case ERoleType::Generalist:
			SelGeneralist->SetIsEnabled(false);
			break;
		case ERoleType::RescueSpecialist:
			SelRescueSpec->SetIsEnabled(false);
			break;
		case ERoleType::Driver:
			SelOperator->SetIsEnabled(false);
			break;
		case ERoleType::Veteran:
			SelVeteran->SetIsEnabled(false);
			break;
		case ERoleType::RescueDog:
			SelRescueDog->SetIsEnabled(false);
			break;
		default:
			break;
		}
	}
}

void USwitchRoleUI::OnCloseCrewChangePanel() {
	if (ensure(crewMan)) {
		// hide the panel using the manager
		crewMan->CloseCrewChangePanel();
	}
}

void USwitchRoleUI::OnSelParamedic() {
	if (ensure(crewMan)) {
		crewMan->SelectRole(ERoleType::Paramedic);
	}
}

void USwitchRoleUI::OnSelImageTec() {
	if (ensure(crewMan)) {
		crewMan->SelectRole(ERoleType::ImagingTechnician);
	}
}

void USwitchRoleUI::OnSelCAFS() {
	if (ensure(crewMan)) {
		crewMan->SelectRole(ERoleType::CAFSFirefighter);
	}
}

void USwitchRoleUI::OnSelHazmatTec() {
	if (ensure(crewMan)) {
		crewMan->SelectRole(ERoleType::HazmatTechnician);
	}
}

void USwitchRoleUI::OnSelVeteran() {
	if (ensure(crewMan)) {
		crewMan->SelectRole(ERoleType::Veteran);
	}
}

void USwitchRoleUI::OnSelRescueDog() {
	if (ensure(crewMan)) {
		crewMan->SelectRole(ERoleType::RescueDog);
	}
}

void USwitchRoleUI::OnSelFireCaptain() {
	if (ensure(crewMan)) {
		crewMan->SelectRole(ERoleType::FireCaptain);
	}
}

void USwitchRoleUI::OnSelGeneralist() {
	if (ensure(crewMan)) {
		crewMan->SelectRole(ERoleType::Generalist);
	}
}

void USwitchRoleUI::OnSelRescueSpec() {
	if (ensure(crewMan)) {
		crewMan->SelectRole(ERoleType::RescueSpecialist);
	}
}

void USwitchRoleUI::OnSelOperator() {
	if (ensure(crewMan)) {
		crewMan->SelectRole(ERoleType::Driver);
	}
}

