// Fill out your copyright notice in the Description page of Project Settings.

#include "FPPlayerController.h"
#include "Victim.h"
#include "Hazmat.h"
#include "Tile.h"
#include "Wall.h"
#include "Door.h"
#include "POI.h"
#include "GameBoard.h"
#include "ChatManager.h"
#include "CrewManager.h"
#include "MenuSystem/OptionPrompt.h"
#include "GeneralTypes.h"
#include "FlashPointSaveGame.h"
#include "FlashPointGameInstance.h"
#include "LobbyManager.h"
#include "MenuSystem/LobbyUI.h"
#include "FireEngine.h"
#include "Ambulance.h"

bool AFPPlayerController::ConsumptionOn()
{
	return apConsumptionOn;
}

AFPPlayerController::AFPPlayerController() {
	// enable mouse clicking and over events
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

}

void AFPPlayerController::SetGameBoard(AGameBoard * inGame)
{
	gameBoard = inGame;
}

AGameBoard * AFPPlayerController::GetGameBoard()
{
	return gameBoard;
}

void AFPPlayerController::SetLobbyManager(ALobbyManager * inMan)
{
	lobbyMan = inMan;
}

ALobbyManager * AFPPlayerController::GetLobbyManager()
{
	return lobbyMan;
}

int32 AFPPlayerController::GetTurnNum()
{
	return myTurnNum;
}

void AFPPlayerController::SetTurnNum(int32 turnNum)
{
	myTurnNum = turnNum;
	UE_LOG(LogTemp, Warning, TEXT("My turn num is: %d"), myTurnNum);
}

void AFPPlayerController::NotifyPlayerTurn()
{
	// restore AP
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn)) {
		int32 currentAP = fireFighterPawn->GetCurrentAP();
		int32 apToRestore = 0;
		if (ensure(gameBoard))
		{
			if (fireFighterPawn->CheckIsVicinty(gameBoard->GetVeteranLoc()))
			{
				currentAP++;
				UE_LOG(LogTemp, Warning, TEXT("Get free AP"));
				fireFighterPawn->SetHasGainedAPThisTurn(true);
			}
		}
		
		if (currentAP + fireFighterPawn->GetRestoreAP() > fireFighterPawn->GetMaxAP()) {
			apToRestore = fireFighterPawn->GetMaxAP() - currentAP;
		}
		else {
			apToRestore = fireFighterPawn->GetRestoreAP();
			if (fireFighterPawn->HasGainedAPThisTurn())
			{
				apToRestore++;
			}
		}
		// if ap to restore is more than 0 restore AP
		if (apToRestore > 0) {
			fireFighterPawn->AdjustFireFighterAP(apToRestore);
		}
		ServerSpecialistTurnAdjust(fireFighterPawn);
	}
	if (ensure(inGameUI)) {
		inGameUI->SetBeginTurnNotify(true);
		inGameUI->NotifyTurnBegin();
	}
}

void AFPPlayerController::NotifyPlayerDodge()
{
	if (ensure(OptionPromptUI))
	{
		UE_LOG(LogTemp, Warning, TEXT("NotifyPlayerDodge"));
		if (ensure(OptionPromptUI)) {
			OptionPromptUI->PromptOperation(EOptionPromptType::Dodge, "You have been knocked down\n please dodge.");
		}
	}
}

void AFPPlayerController::EndPlayerTurn()
{
	// disable all operations
	SetNone();
	if (ensure(inGameUI)) {
		inGameUI->NotifyTurnEnd();
	}
	if (HasAuthority()) {
		if (ensure(gameBoard)) {
			gameBoard->TurnSwitch();
		}
	}
	else {
		ServerEndTurn(gameBoard);
	}
}

UFireFighterStatus* AFPPlayerController::GetFireFighterStatusBar(int32 id)
{
	if (!ensure(inGameUI)) return nullptr;
	return inGameUI->GetPlayerStatusUIOf(id);
}

void AFPPlayerController::NotifyGameOver(bool isWin)
{
	if (ensure(inGameUI)) {
		inGameUI->NotifyGameEnd(isWin);
	}
}

void AFPPlayerController::SetPlayerName(FString inName)
{
	playerName = inName;
}

FString AFPPlayerController::GetPlayerName()
{
	return playerName;
}

void AFPPlayerController::EnableOperations(bool enable)
{
	if (ensure(inGameUI)) {
		inGameUI->EnableOperationPanels(enable);
	}
}

void AFPPlayerController::PromtCrewChange()
{
	// only do things if the game mode is not family when in game
	if (gameBoard) {
		if (gameBoard->GetGameType() != EGameType::Family) {
			// check if the player's firefighter is at correct locations
			AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
			if (!ensure(fireFighterPawn)) return;
			if (!ensure(crewMan)) return;
			//if (!fireFighterPawn->IsWithEngine()) return;
			crewMan->ShowCrewChangeUI();
		}
	}
}

void AFPPlayerController::PromtCommandStatus(EAcceptanceStatus commandStatus)
{
	if (!ensure(inGameUI)) return;
	inGameUI->SetBeginTurnNotify(false);
	inGameUI->PromptCommandStatus(commandStatus);
	switch (commandStatus)
	{
	case EAcceptanceStatus::Accepted:
		UE_LOG(LogTemp, Warning, TEXT("Prompt Accepted"))
			break;
	case EAcceptanceStatus::Rejected:
		UE_LOG(LogTemp, Warning, TEXT("Prompt Rejected"))
			break;
	case EAcceptanceStatus::Waiting:
		UE_LOG(LogTemp, Warning, TEXT("Prompt Waiting"))
			break;
	case EAcceptanceStatus::Empty:
		UE_LOG(LogTemp, Warning, TEXT("Prompt Empty"))
			break;
	default:
		break;
	}
}

void AFPPlayerController::PromtOptionPanel(EOptionPromptType option, FString optionText)
{
	if (ensure(OptionPromptUI)) {
		OptionPromptUI->PromptOperation(option, optionText);
	}
}

void AFPPlayerController::ServerChopWall_Implementation(AWall * wall)
{
	if (ensure(wall)) {
		wall->ChopWall();
	}
	//ClientWallMeshUpdate(wall, !wall->IsBlocked());
}

bool AFPPlayerController::ServerChopWall_Validate(AWall * wall)
{
	return true;
}

void AFPPlayerController::ServerOpenDoor_Implementation(ADoor * door)
{
	if (ensure(door)) {
		door->ChangeDoorStatus();
	}
}

bool AFPPlayerController::ServerOpenDoor_Validate(ADoor * door)
{
	return true;
}

void AFPPlayerController::ServerMovePawn_Implementation(ATile * tile, AFireFighterPawn * pawnToMove, const TArray<ATile*>& trace)
{
	if (ensure(tile)) {
		tile->PawnMoveToHere(pawnToMove, trace);
	}
}

bool AFPPlayerController::ServerMovePawn_Validate(ATile * tile, AFireFighterPawn * pawnToMove, const TArray<ATile*>& trace)
{
	return true;
}

void AFPPlayerController::ServerPlacePawn_Implementation(ATile * tile, AFireFighterPawn * pawnToPlace)
{
	if (ensure(tile)) {
		tile->PlacePawnHere(pawnToPlace);
		AGameBoard* tempBoard = tile->GetGameBoard();
		if (ensure(tempBoard)) {
			tempBoard->InitialPlacing();
		}
	}
}

bool AFPPlayerController::ServerPlacePawn_Validate(ATile * tile, AFireFighterPawn * pawnToPlace)
{
	return true;
}

void AFPPlayerController::ServerExtinguishFire_Implementation(ATile * tile)
{
	if (ensure(tile)) {
		tile->ExitinguishFireOnTile();
	}
}

bool AFPPlayerController::ServerExtinguishFire_Validate(ATile * tile)
{
	return true;
}

void AFPPlayerController::ServerAdvanceFire_Implementation(AGameBoard * board)
{
	if (ensure(board)) {
		board->AdvanceFireOnBoard();
	}
}

bool AFPPlayerController::ServerAdvanceFire_Validate(AGameBoard * board)
{
	return true;
}

void AFPPlayerController::ServerAdvancePOI_Implementation(AGameBoard * board)
{
	if (ensure(board)) {
		board->AdvancePOIOnBoard();
	}
}

bool AFPPlayerController::ServerAdvancePOI_Validate(AGameBoard * board)
{
	return true;
}

void AFPPlayerController::ServerDropVictim_Implementation(AFireFighterPawn * fireFighterPawn)
{
	ATile* currentTile = fireFighterPawn->GetPlacedOn();
	UE_LOG(LogTemp, Warning, TEXT("SreverDrop"));
	if (ensure(currentTile) && currentTile->GetGameBoard()->GetGameType() == EGameType::Family)
	{
		// if the tile is on fire, just return
		if (currentTile->GetFireStatus() == EFireStatus::Fire)	return;
		AVictim* tempVictim = fireFighterPawn->GetVictim();
		if (!tempVictim) return;
		if (currentTile->IsOutside())
		{
			tempVictim->Destroy();
			UE_LOG(LogTemp, Warning, TEXT("Outside"));
			currentTile->SetPOIStatus(EPOIStatus::Empty);
			currentTile->GetGameBoard()->SetVictimSavedNum(
				currentTile->GetGameBoard()->victimSavedNum + 1
			);
			UE_LOG(LogTemp, Warning, TEXT("Current saved victim: %d"), currentTile->GetGameBoard()->victimSavedNum);
			currentTile->GetGameBoard()->SetCurrentPOI(
				currentTile->GetGameBoard()->currentPOI - 1
			);
			UE_LOG(LogTemp, Warning, TEXT("Current saved victim: %d"), currentTile->GetGameBoard()->currentPOI);
		}
		else {
			tempVictim->victimMesh->SetVisibility(true);
			FVector VictimSocketLocation;
			switch (currentTile->GetVictims()->Num())
			{
			case 0:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
				break;
			case 1:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim1"));
				break;
			case 2:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim3"));
				break;
			case 3:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim4"));
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("No more position"))
					VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
				break;
			}

			tempVictim->victimMesh->SetRelativeLocation(VictimSocketLocation);
			tempVictim->SetVictimLoc(tempVictim->GetActorLocation());

			UE_LOG(LogTemp, Warning, TEXT("Before Add Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());
			currentTile->GetVictims()->Add(fireFighterPawn->GetVictim());
			UE_LOG(LogTemp, Warning, TEXT("Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());

			currentTile->SetPOIStatus(EPOIStatus::Revealed);
		}
		fireFighterPawn->SetCarriedVictim(nullptr);
		tempVictim->SetIsCarried(false);

		// only for server, actively check if the game is won
		if (HasAuthority()) {
			AGameBoard* tempBoard = currentTile->GetGameBoard();
			if (ensure(tempBoard)) {
				if (tempBoard->victimSavedNum >= tempBoard->maxSavedVictim) {
					AFPPlayerController* tempPlayer = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					if (ensure(tempPlayer)) {
						tempPlayer->NotifyGameOver(true);
					}
				}
			}
		}
	}
	else if (ensure(currentTile) && currentTile->GetGameBoard()->GetGameType() == EGameType::Experienced)
	{
		// if the tile is on fire, just return
		if (currentTile->GetFireStatus() == EFireStatus::Fire)	return;
		UE_LOG(LogTemp, Warning, TEXT("Drop unhealed victim"));
		AVictim* tempVictim = fireFighterPawn->GetVictim();
		if (!tempVictim) return;
		if (currentTile == currentTile->GetGameBoard()->ambulanceLocA || currentTile == currentTile->GetGameBoard()->ambulanceLocB)
		{
			tempVictim->Destroy();
			currentTile->SetPOIStatus(EPOIStatus::Empty);
			currentTile->GetGameBoard()->SetVictimSavedNum(
				currentTile->GetGameBoard()->victimSavedNum + 1
			);
			UE_LOG(LogTemp, Warning, TEXT("Current saved victim: %d"), currentTile->GetGameBoard()->victimSavedNum);
			currentTile->GetGameBoard()->SetCurrentPOI(
				currentTile->GetGameBoard()->currentPOI - 1
			);
			UE_LOG(LogTemp, Warning, TEXT("Current saved victim: %d"), currentTile->GetGameBoard()->currentPOI);
		}
		else {
			tempVictim->victimMesh->SetVisibility(true);
			FVector VictimSocketLocation;
			switch (currentTile->GetVictims()->Num())
			{
			case 0:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
				break;
			case 1:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim1"));
				break;
			case 2:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim3"));
				break;
			case 3:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim4"));
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("No more position"))
					VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
				break;
			}

			tempVictim->victimMesh->SetRelativeLocation(VictimSocketLocation);
			tempVictim->SetVictimLoc(tempVictim->GetActorLocation());

			UE_LOG(LogTemp, Warning, TEXT("Before Add Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());
			currentTile->GetVictims()->Add(fireFighterPawn->GetVictim());
			UE_LOG(LogTemp, Warning, TEXT("Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());

			currentTile->SetPOIStatus(EPOIStatus::Revealed);
		}

		if (tempVictim->IsHealed())
		{
			fireFighterPawn->SetLeading(nullptr);
			UE_LOG(LogTemp, Warning, TEXT("No lead anymore"));
		}
		else
		{
			fireFighterPawn->SetCarriedVictim(nullptr);
			tempVictim->SetIsCarried(false);
			UE_LOG(LogTemp, Warning, TEXT("Drop unhealed victim"));
		}

		// only for server, actively check if the game is won
		if (HasAuthority()) {
			AGameBoard* tempBoard = currentTile->GetGameBoard();
			if (ensure(tempBoard)) {
				if (tempBoard->victimSavedNum >= tempBoard->maxSavedVictim) {
					AFPPlayerController* tempPlayer = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					if (ensure(tempPlayer)) {
						tempPlayer->NotifyGameOver(true);
					}
				}
			}
		}
	}
}

bool AFPPlayerController::ServerDropVictim_Validate(AFireFighterPawn * fireFighterPawn)
{
	return true;
}

void AFPPlayerController::ServerCarryVictim_Implementation(AFireFighterPawn * fireFighterPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Server carry"));
	ATile* currentTile = fireFighterPawn->GetPlacedOn();
	if (ensure(currentTile))
	{
		if (currentTile->GetPOIStatus() == EPOIStatus::Revealed)
		{
			if (currentTile->GetVictims()->Num() > 0)
			{
				AVictim* targetVictim = currentTile->GetVictims()->Pop(true);
				if (!(targetVictim)) { return; }
				if (targetVictim->IsHealed())
				{
					if (ensure(fireFighterPawn))
					{
						if (fireFighterPawn->GetLeading() == nullptr)
						{
							fireFighterPawn->SetLeading(targetVictim);
							targetVictim->SetActorLocation(fireFighterPawn->GetActorLocation() - FVector(0, 100, 0));
							targetVictim->SetVictimLoc(targetVictim->GetActorLocation());
							UE_LOG(LogTemp, Warning, TEXT("Victim has been lead"));
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Already leading"));
						}
					}
				}
				else
				{
					if (!ensure(targetVictim)) return;
					targetVictim->SetIsCarried(true);
					UE_LOG(LogTemp, Warning, TEXT("After pop(). Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());
					fireFighterPawn->SetCarriedVictim(targetVictim);
					targetVictim->victimMesh->SetVisibility(false);
					if (currentTile->GetVictims()->Num() == 0)
					{
						currentTile->SetPOIStatus(EPOIStatus::Empty);
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No victim."));
			}
		}
	}
	if (fireFighterPawn->GetLeading())
	{
		if (ensure(fireFighterPawn->GetLeading()->GetPlacedOn()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Leading Victim correct"));
		}
	}
	
}

bool AFPPlayerController::ServerCarryVictim_Validate(AFireFighterPawn * fireFighterPawn)
{
	return true;
}

void AFPPlayerController::ServerCarryHazmat_Implementation(AFireFighterPawn * fireFighterPawn)
{
	if (!ensure(fireFighterPawn))	return;
	ATile* currentTile = fireFighterPawn->GetPlacedOn();
	if (ensure(currentTile))
	{
		AHazmat* carriedHazmat = currentTile->GetHazmat();
		if (carriedHazmat)
		{
			carriedHazmat->isCarried = true;
			fireFighterPawn->SetHazmat(carriedHazmat);
			if (ensure(carriedHazmat->hazmatMesh)) {
				carriedHazmat->hazmatMesh->SetVisibility(false);
			}
			UE_LOG(LogTemp, Warning, TEXT("Carried hazmat."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No hazmat."));
		}
	}
}

bool AFPPlayerController::ServerCarryHazmat_Validate(AFireFighterPawn * fireFighterPawn)
{
	return true;
}

void AFPPlayerController::ServerDropHazmat_Implementation(AFireFighterPawn * fireFighterPawn)
{
	ATile* currentTile = fireFighterPawn->GetPlacedOn();
	if (ensure(currentTile))
	{
		if (currentTile->GetFireStatus() == EFireStatus::Fire) return;
		AHazmat* tempHazmat = fireFighterPawn->GetHazmat();
		if (!tempHazmat) return;
		if (currentTile->IsOutside())
		{
			tempHazmat->Destroy();
			UE_LOG(LogTemp, Warning, TEXT("Hazmat Dropped Outside"));
		}
		else
		{
			tempHazmat->hazmatMesh->SetVisibility(true);
			FVector HazmatSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("VisualEffects"));
			tempHazmat->hazmatMesh->SetRelativeLocation(HazmatSocketLocation);
			int32 xLoc, yLoc;
			currentTile->GetLocation(xLoc, yLoc);
			tempHazmat->SetHazmatLoc(tempHazmat->GetActorLocation());

			currentTile->SetHazmatOnTile(tempHazmat);
		}
		fireFighterPawn->SetHazmat(nullptr);
		tempHazmat->isCarried = false;
	}
}

bool AFPPlayerController::ServerDropHazmat_Validate(AFireFighterPawn * fireFighterPawn)
{
	return true;
}

void AFPPlayerController::ServerGetFireFighterID_Implementation(AFireFighterPawn * fireFighterPawn, AGameBoard * inGameBoard)
{
	if (ensure(fireFighterPawn) && ensure(inGameBoard)) {
		fireFighterPawn->SetFireFighterID(inGameBoard->JoinBoard());
	}
}

bool AFPPlayerController::ServerGetFireFighterID_Validate(AFireFighterPawn * fireFighterPawn, AGameBoard * inGameBoard)
{
	return true;
}

void AFPPlayerController::ServerEndTurn_Implementation(AGameBoard * inGameBoard)
{
	if (ensure(inGameBoard)) {
		inGameBoard->TurnSwitch();
	}
}

bool AFPPlayerController::ServerEndTurn_Validate(AGameBoard * inGameBoard)
{
	return true;
}

void AFPPlayerController::ServerAdjustAP_Implementation(AFireFighterPawn* fireFighterPawn, int32 adjustAP)
{
	if (ensure(fireFighterPawn)) {
		int32 currentAP = fireFighterPawn->GetCurrentAP();
		fireFighterPawn->SetCurrentAP(currentAP + adjustAP);
	}
}

bool AFPPlayerController::ServerAdjustAP_Validate(AFireFighterPawn* fireFighterPawn, int32 adjustAP)
{
	return true;
}

void AFPPlayerController::ServerAdjustCAFSAP_Implementation(AFireFighterPawn * fireFighterPawn, int32 adjustAP)
{
	if (!ensure(fireFighterPawn)) return;
	if (fireFighterPawn->GetExtinguishAP() < -adjustAP)
	{
		adjustAP += fireFighterPawn->GetExtinguishAP();
		fireFighterPawn->SetExtinguishAP(0);
		fireFighterPawn->SetCurrentAP(fireFighterPawn->GetCurrentAP() + adjustAP);
	}
	else
	{
		fireFighterPawn->SetExtinguishAP(fireFighterPawn->GetExtinguishAP() + adjustAP);
	}
}

bool AFPPlayerController::ServerAdjustCAFSAP_Validate(AFireFighterPawn * fireFighterPawn, int32 adjustAP)
{
	return true;
}

void AFPPlayerController::ServerSendGlobalText_Implementation(AChatManager * chatMan, const FString& message)
{
	if (ensure(chatMan)) {
		chatMan->SendGlobalText(message);
	}
}

bool AFPPlayerController::ServerSendGlobalText_Validate(AChatManager * chatMan, const FString& message)
{
	return true;
}

void AFPPlayerController::ServerSetFireFighterName_Implementation(AFireFighterPawn * fireFighterPawn, const FString & inName)
{
	if (ensure(fireFighterPawn)) {
		fireFighterPawn->SetFireFighterName(inName);
	}
}

bool AFPPlayerController::ServerSetFireFighterName_Validate(AFireFighterPawn * fireFighterPawn, const FString & inName)
{
	return true;
}

void AFPPlayerController::ServerSwitchRole_Implementation(ACrewManager * inCrewMan, AFireFighterPawn * fireFighterPawn, ERoleType inRole)
{
	if (ensure(inCrewMan) && ensure(fireFighterPawn)) {
		inCrewMan->SwitchRolesFromTo(fireFighterPawn->GetFireFighterRole(), inRole);
		fireFighterPawn->SetFireFighterRole(inRole);
		fireFighterPawn->AdjustRoleProperties(inRole);
	}
}

bool AFPPlayerController::ServerSwitchRole_Validate(ACrewManager * inCrewMan, AFireFighterPawn * fireFighterPawn, ERoleType inRole)
{
	return true;
}

void AFPPlayerController::ServerSpecialistTurnAdjust_Implementation(AFireFighterPawn * fireFighterPawn)
{
	if (!ensure(fireFighterPawn)) return;
	// for special roles, also restore special AP
	if (fireFighterPawn->GetFireFighterRole() == ERoleType::FireCaptain) {
		// The command AP of the firefighter is fixed to be 2 and cannot be saved thus this is 2
		fireFighterPawn->SetCommandAP(2);
	}
	if (fireFighterPawn->GetFireFighterRole() == ERoleType::CAFSFirefighter) {
		// The extinguish Ap cannot be saved and each round there will be 3
		fireFighterPawn->SetExtinguishAP(3);
	}
	if (fireFighterPawn->GetFireFighterRole() == ERoleType::RescueSpecialist) {
		// The movement Ap cannot be saved and each round there will be 3
		fireFighterPawn->SetMovementAP(3);
	}
}

bool AFPPlayerController::ServerSpecialistTurnAdjust_Validate(AFireFighterPawn * fireFighterPawn)
{
	return true;
}

void AFPPlayerController::ServerCommandDoorOperation_Implementation(AFireFighterPawn * fireFighterPawn, AFireFighterPawn * captain, ADoor * target)
{
	if (ensure(fireFighterPawn) && ensure(captain)) {
		fireFighterPawn->CommandDoorOperation(target, captain);
		// as the operation is sent to the player, captain should be waiting for the command to be done
		if (!captain) { return; }
		captain->SetCommandAcceptance(EAcceptanceStatus::Waiting);
		// if the captain is local pawn, just notify its own
		UWorld* world = GetWorld();
		if (ensure(world)) {
			// only do this on local commanded pawn
			AFPPlayerController* localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
			if (ensure(localPlayer) && localPlayer->GetPawn() == captain) {
				localPlayer->PromtCommandStatus(EAcceptanceStatus::Waiting);
			}
		}
	}
}

bool AFPPlayerController::ServerCommandDoorOperation_Validate(AFireFighterPawn * fireFighterPawn, AFireFighterPawn * captain, ADoor * target)
{
	return true;
}

void AFPPlayerController::ServerCommandTileOperation_Implementation(AFireFighterPawn * fireFighterPawn, AFireFighterPawn * captain, const TArray<ATile*>& targets)
{
	if (ensure(fireFighterPawn)) {
		fireFighterPawn->CommandTileOperation(targets, captain);
		// as the operation is sent to the player, captain should be waiting for the command to be done
		if (!captain) { return; }
		captain->SetCommandAcceptance(EAcceptanceStatus::Waiting);
		// if the captain is local pawn, just notify its own
		UWorld* world = GetWorld();
		if (ensure(world)) {
			// only do this on local commanded pawn
			AFPPlayerController* localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
			if (ensure(localPlayer) && localPlayer->GetPawn() == captain) {
				localPlayer->PromtCommandStatus(EAcceptanceStatus::Waiting);
			}
		}
	}
}

bool AFPPlayerController::ServerCommandTileOperation_Validate(AFireFighterPawn * fireFighterPawn, AFireFighterPawn * captain, const TArray<ATile*>& targets)
{
	return true;
}

void AFPPlayerController::ServerSetCommandStatus_Implementation(AFireFighterPawn * captain, EAcceptanceStatus inStatus)
{
	if (ensure(captain)) {
		if (!(captain->GetFireFighterRole() == ERoleType::FireCaptain)) return;
		captain->SetCommandAcceptance(inStatus);
		// figure out if it is local player pawn
		UWorld* world = GetWorld();
		if (ensure(world)) {
			// only do this on local commanded pawn
			AFPPlayerController* localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
			if (ensure(localPlayer) && localPlayer->GetPawn() == captain) {

				// if it is my own firefighter, clear operations
				if (inStatus == EAcceptanceStatus::Empty) {
					SetNone();
					CancelCommand();
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("server prompt"));
					localPlayer->PromtCommandStatus(inStatus);
				}
			}
		}
	}
}

bool AFPPlayerController::ServerSetCommandStatus_Validate(AFireFighterPawn * captain, EAcceptanceStatus inStatus)
{
	return true;
}

void AFPPlayerController::ServerJoinLobby_Implementation(ALobbyManager * inMan, AFireFighterPawn * inPawn, const FString& inName)
{
	if (ensure(inMan)) {
		UE_LOG(LogTemp, Warning, TEXT("Joining Lobby"));
		inMan->AutoJoinLobby(inPawn, inName);
	}
}

bool AFPPlayerController::ServerJoinLobby_Validate(ALobbyManager * inMan, AFireFighterPawn * inPawn, const FString& inName)
{
	return true;
}

void AFPPlayerController::ServerChangeReadyStatus_Implementation(ALobbyManager * inMan, AFireFighterPawn * inPawn)
{
	if (ensure(inMan) && ensure(inPawn)) {
		FPlayerLobbyInfo tempInfo = inMan->GetPlayerLobbyInfo(inPawn->GetFireFighterLobbyID());
		// switching the player's join status
		if (tempInfo.joinStatus == EJoinStatus::Ready) {
			inMan->UpdatePlayerJoinStatus(inPawn, EJoinStatus::Waiting);
		}
		else if (tempInfo.joinStatus == EJoinStatus::Waiting) {
			inMan->UpdatePlayerJoinStatus(inPawn, EJoinStatus::Ready);
		}
	}
}

bool AFPPlayerController::ServerChangeReadyStatus_Validate(ALobbyManager * inMan, AFireFighterPawn * inPawn)
{
	return true;
}

void AFPPlayerController::ServerExitFromLobby_Implementation(ALobbyManager * inMan, AFireFighterPawn * inPawn)
{
	if (!ensure(inMan) || !ensure(inPawn)) return;
	inMan->ExitLobby(inPawn);
}

bool AFPPlayerController::ServerExitFromLobby_Validate(ALobbyManager * inMan, AFireFighterPawn * inPawn)
{
	return true;
}

void AFPPlayerController::ServerGetInCar_Implementation(AFireFighterPawn * inPawn)
{
	if (ensure(inPawn))
	{
		inPawn->SetIsInCar(true);
		if (ensure(inPawn->GetPlacedOn()))
		{
			inPawn->GetPlacedOn()->GetPlacedFireFighters()->Remove(inPawn);
		}
		inPawn->SetPlacedOn(nullptr);
		inPawn->SetVisibility(false);
	}
}

bool AFPPlayerController::ServerGetInCar_Validate(AFireFighterPawn * inPawn)
{
	return true;
}

void AFPPlayerController::ServerFireDeckGun_Implementation(AFireFighterPawn * inPawn)
{
	if (ensure(inPawn))
	{
		AGameBoard* board = inPawn->GetPlayingBoard();
		if (ensure(board))
		{
			AFireEngine* fireEngine = board->GetFireEngine();
			if (ensure(fireEngine))
			{
				fireEngine->FireDeckGun();
			}
		}
	}
}

bool AFPPlayerController::ServerFireDeckGun_Validate(AFireFighterPawn * inPawn)
{
	return true;
}

void AFPPlayerController::ServerGetOutAmbulance_Implementation(AFireFighterPawn * inPawn, ATile * current, AAmbulance * inAmbulance)
{
	if (ensure(inPawn) && ensure(current) && ensure(inAmbulance))
	{
		inPawn->SetIsInCar(false);
		current->GetPlacedFireFighters()->Add(inPawn);
		inAmbulance->GetPassengers()->Remove(inPawn);
		inPawn->SetPlacedOn(current);
		inPawn->SetActorLocation(current->GetTileMesh()->GetSocketLocation(FName("VisualEffects")));
		inPawn->SetVisibility(true);
	}
}

bool AFPPlayerController::ServerGetOutAmbulance_Validate(AFireFighterPawn * inPawn, ATile * current, AAmbulance * inAmbulance)
{
	return true;
}

bool AFPPlayerController::ServerGetOutFireEngine_Validate(AFireFighterPawn * inPawn, ATile * current, AFireEngine * inFireEngine)
{
	return true;
}

void AFPPlayerController::ServerGetOutFireEngine_Implementation(AFireFighterPawn * inPawn, ATile * current, AFireEngine * inFireEngine)
{
	if (ensure(inPawn) && ensure(current) && ensure(inFireEngine))
	{
		inPawn->SetIsInCar(false);
		current->GetPlacedFireFighters()->Add(inPawn);
		inFireEngine->GetPassengers()->Remove(inPawn);
		inPawn->SetPlacedOn(current);
		inPawn->SetActorLocation(current->GetTileMesh()->GetSocketLocation(FName("VisualEffects")));
		inPawn->SetVisibility(true);
	}
}

void AFPPlayerController::DropVictim()
{
	UE_LOG(LogTemp, Warning, TEXT("Drop victim."));
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
		/*
		// for client only, do the victim position visual settings
		if (!HasAuthority()) {
			ATile* currentTile = fireFighterPawn->GetPlacedOn();
			if (!ensure(currentTile)) return;
			FVector VictimSocketLocation;
			switch (currentTile->GetVictims()->Num())
			{
			case 0:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
				break;
			case 1:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim1"));
				break;
			case 2:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim3"));
				break;
			case 3:
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim4"));
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("No more position"));
				VictimSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
				break;
			}
			AVictim* tempVictim = fireFighterPawn->GetCarriedVictim();
			if (!ensure(tempVictim)) return;
			tempVictim->victimMesh->SetRelativeLocation(VictimSocketLocation);
		}
		*/
		ServerDropVictim(fireFighterPawn);
		// only for authority, update the firefighter's carrying UI
		if (HasAuthority()) {
			if (ensure(inGameUI)) {
				if (fireFighterPawn->GetCarriedVictim()) {
					inGameUI->ShowCarrying(true);
				}
				else {
					inGameUI->ShowCarrying(false);
				}
			}
		}
	}
}

void AFPPlayerController::CarryVictim()
{
	UE_LOG(LogTemp, Warning, TEXT("Carry victim."));

	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
		ServerCarryVictim(fireFighterPawn);
		// only for server, update the UI actively
		if (HasAuthority()) {
			if (ensure(inGameUI)) {
				// show carrying victim
				if (fireFighterPawn->GetCarriedVictim()) {
					inGameUI->ShowCarrying(true);
				}
				else {
					inGameUI->ShowCarrying(false);
				}
				// show leading victim
				if (fireFighterPawn->GetLeading()) {
					inGameUI->ShowLeading(true);
				}
				else {
					inGameUI->ShowLeading(false);
				}
			}
		}
	}

}

void AFPPlayerController::DropHazmat()
{
	// TODO drop hazmat from pawn to tile when called

	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
		if (!HasAuthority())
		{
			ATile* currentTile = fireFighterPawn->GetPlacedOn();
			if (!ensure(currentTile)) return;
			FVector HazmatSocketLocation = currentTile->GetTileMesh()->GetSocketLocation(FName("VisualEffects"));
			AHazmat* tempHazmat = fireFighterPawn->GetHazmat();
			if (!ensure(tempHazmat)) return;
			tempHazmat->hazmatMesh->SetRelativeLocation(HazmatSocketLocation);
		}
		ServerDropHazmat(fireFighterPawn);
		if (HasAuthority())
		{
			if (ensure(inGameUI))
			{
				if (fireFighterPawn->GetHazmat())
				{
					inGameUI->ShowCarryHazmat(true);
				}
				else
				{
					inGameUI->ShowCarryHazmat(false);
				}
			}
		}
	}
}

void AFPPlayerController::CarryHazmat()
{
	// TODO carry hazmat on tile when called
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
		ServerCarryHazmat(fireFighterPawn);
		// only for server, update the UI actively
		if (HasAuthority()) {
			if (ensure(inGameUI)) {
				if (fireFighterPawn->GetHazmat()) {
					inGameUI->ShowCarryHazmat(true);
				}
				else {
					inGameUI->ShowCarryHazmat(false);
				}
			}
		}
		// if (!(fireFighterPawn->GetFireFighterRole() != ERoleType::HazmatTechnician)) { return; }
	}
}

void AFPPlayerController::HealVictim()
{
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
		if (fireFighterPawn->GetFireFighterRole() != ERoleType::Paramedic) { return; }
		ATile* currentTile = fireFighterPawn->GetPlacedOn();
		if (ensure(currentTile))
		{
			if (currentTile->GetVictims()->Num() == 0) { return; }
			bool healed = false;
			for (size_t i = 0; i < currentTile->GetVictims()->Num(); i++)
			{
				if (!(*currentTile->GetVictims())[i]->IsHealed())
				{
					ServerHealVictim(fireFighterPawn, (*currentTile->GetVictims())[i]);
					// for the healed victim, show its healed marker

					healed = true;
					break;
				}
			}
			if (healed)
			{
				UE_LOG(LogTemp, Warning, TEXT("A victim has been healed"));
				fireFighterPawn->AdjustFireFighterAP(-fireFighterPawn->GetHealConsumption());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("All victim has already been healed"));
			}
		}
	}
}

void AFPPlayerController::RemoveHazmat()
{
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
		if (ensure(fireFighterPawn->GetFireFighterRole() == ERoleType::HazmatTechnician))
		{
			ATile* currentTile = fireFighterPawn->GetPlacedOn();
			if (ensure(currentTile))
			{
				if (currentTile->GetHazmat() != nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("Reveal Hazmat."));
					fireFighterPawn->GetPlayingBoard()->removedHazmat++;
					currentTile->GetHazmat()->Destroy();
					currentTile->SetHazmatOnTile(nullptr);
				}
			}
		}
	}
}

//Todo Refactor
bool AFPPlayerController::GetInAmbulance() {
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn)) {
		if (fireFighterPawn->IsWithAmbulance()) {
			fireFighterPawn->SetVisibility(false);
			gameBoard->moved = false;
			return true;
		}
	}
	return false;
}
void AFPPlayerController::GetOutAmbulance() {
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn)) {
		if (fireFighterPawn->IsWithAmbulance()) {
			fireFighterPawn->SetVisibility(true);
			gameBoard->moved = true;
		}
	}
}

bool AFPPlayerController::GetInFireEngine() {
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn)) {
		if (fireFighterPawn->IsWithEngine()) {
			fireFighterPawn->SetVisibility(false);
			gameBoard->moved = false;
			return true;
		}
	}
	return false;
}
void AFPPlayerController::GetOutFireEngine() {
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn)) {
		if (fireFighterPawn->IsWithEngine()) {
			fireFighterPawn->SetVisibility(true);
			gameBoard->moved = true;
		}
	}
}


void AFPPlayerController::Dodge()
{
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (fireFighterPawn->GetCanDodge()) {
		UE_LOG(LogTemp, Warning, TEXT("Can dodge."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can not dodge."));
	}
}

void AFPPlayerController::ServerRevealPOI_Implementation(ATile* targetTile)
{
	UE_LOG(LogTemp, Warning, TEXT("Reveal POI."));
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
		if (fireFighterPawn->GetFireFighterRole() == ERoleType::ImagingTechnician
			|| fireFighterPawn->GetFireFighterRole() == ERoleType::RescueDog)
		{
			if (ensure(targetTile))
			{
				if (ensure(fireFighterPawn->GetFireFighterRole() == ERoleType::ImagingTechnician || targetTile->AdjacentToPawn(fireFighterPawn)))
				{
					if (targetTile->GetPOIStatus() == EPOIStatus::Hided)
					{
						UE_LOG(LogTemp, Warning, TEXT("A POI has been revealed."));
						if (!ensure(targetTile->GetPOIOnTile())) return;
						if (targetTile->GetPOIOnTile()->isAlarm)
						{
							FVector VictimSocketLocation;
							switch (targetTile->GetVictims()->Num())
							{
							case 0:
								VictimSocketLocation = targetTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
								break;
							case 1:
								VictimSocketLocation = targetTile->GetTileMesh()->GetSocketLocation(FName("Victim1"));
								break;
							case 2:
								VictimSocketLocation = targetTile->GetTileMesh()->GetSocketLocation(FName("Victim3"));
								break;
							case 3:
								VictimSocketLocation = targetTile->GetTileMesh()->GetSocketLocation(FName("Victim4"));
								break;
							default:
								UE_LOG(LogTemp, Warning, TEXT("No more position"))
									VictimSocketLocation = targetTile->GetTileMesh()->GetSocketLocation(FName("Victim"));
								break;
							}
							AVictim* newVictim = GetWorld()->SpawnActor<AVictim>(
								targetTile->GetVictimClass(),
								VictimSocketLocation,
								FRotator(0, 0, 0)
								);
							newVictim->SetPlacedOn(targetTile);
							if (ensure(newVictim))
							{
								targetTile->GetVictims()->Add(newVictim);
							}
							targetTile->SetPOIStatus(EPOIStatus::Revealed);
						}
						else
						{
							targetTile->SetPOIStatus(EPOIStatus::Empty);
							targetTile->GetGameBoard()->SetCurrentPOI(GetGameBoard()->currentPOI - 1);
						}
						targetTile->GetPOIOnTile()->Destroy();
						targetTile->SetPOIOnTile(nullptr);
						if (fireFighterPawn->GetFireFighterRole() == ERoleType::ImagingTechnician)
						{
							fireFighterPawn->AdjustFireFighterAP(-fireFighterPawn->GetFlipConsumption());
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("No POI to reveal"));
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Cannot reveal"));
				}
			}
		}
	}
}

bool AFPPlayerController::ServerRevealPOI_Validate(ATile* targetTile) {
	return true;
}

AFireFighterPawn * AFPPlayerController::GetCommanded()
{
	return commandedPawn;
}

void AFPPlayerController::SetCommanded(AFireFighterPawn * inFireFighter)
{
	commandedPawn = inFireFighter;
	// when commandeding a firefighter, abilities and skip turn panel should be disabled until command done or canceled
}

void AFPPlayerController::FindGameBoard()
{
	// ensure there is a world
	UWorld* world = GetWorld();
	if (ensure(world)) {
		TArray<AActor*> allGameBoard;
		UGameplayStatics::GetAllActorsOfClass(world, AGameBoard::StaticClass(), allGameBoard);
		// only assign correct game board if there is one found
		if (allGameBoard.Num() > 0) {
			UE_LOG(LogTemp, Warning, TEXT("Player found gameboard"));
			gameBoard = Cast<AGameBoard>(allGameBoard[0]);
		}
	}
}

void AFPPlayerController::FindLobbyManager()
{
	// ensure there is a world
	UWorld* world = GetWorld();
	if (ensure(world)) {
		TArray<AActor*> allLobbyMan;
		UGameplayStatics::GetAllActorsOfClass(world, ALobbyManager::StaticClass(), allLobbyMan);
		// only assign correct game board if there is one found
		if (allLobbyMan.Num() > 0) {
			UE_LOG(LogTemp, Warning, TEXT("Player found lobby"));
			lobbyMan = Cast<ALobbyManager>(allLobbyMan[0]);
		}
	}
}

void AFPPlayerController::RelateInGameUI(AFireFighterPawn * fireFighterPawn)
{
	if (ensure(inGameUI)) {
		inGameUI->SetGameBoard(gameBoard);
		inGameUI->SetRelatedPlayer(this);
		inGameUI->SetRelatedPawn(fireFighterPawn);
	}
}

void AFPPlayerController::MakeBasicFireFighterUI()
{
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn) && ensure(gameBoard))
	{
		if (ensure(BasicFireFighterClass)) {
			inGameUI = CreateWidget<UFireFighterUI>(this, BasicFireFighterClass);
			RelateInGameUI(fireFighterPawn);
			if (ensure(inGameUI)) {
				inGameUI->AddToViewport();
			}
		}
	}
}

void AFPPlayerController::MakeOptionPromptUI()
{
	if (!IsLocalController()) return;
	if (ensure(gameBoard))
	{
		if (ensure(OptionClass)) {
			OptionPromptUI = CreateWidget<UOptionPrompt>(this, OptionClass);
			if (ensure(OptionPromptUI)) {
				OptionPromptUI->AddToViewport();
				OptionPromptUI->AssociatePlayer(this);
				OptionPromptUI->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void AFPPlayerController::EnableAPConsumption(int32 flag)
{
	if (flag) {
		apConsumptionOn = true;
		if (!ensure(GEngine)) return;
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("AP Consumption Enabled"));
	}
	else {
		apConsumptionOn = false;
		if (!ensure(GEngine)) return;
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("AP Consumption Disabled"));
	}
}

void AFPPlayerController::SaveCurrentGame()
{
	if (ensure(gameBoard)) {
		UFlashPointSaveGame* savedGames;
		if (UGameplayStatics::DoesSaveGameExist(FString(TEXT("SaveSlot")), 0)) {
			UE_LOG(LogTemp, Warning, TEXT("Found the saved game"));
			savedGames = Cast<UFlashPointSaveGame>(UGameplayStatics::LoadGameFromSlot(FString(TEXT("SaveSlot")), 0));
			if (ensure(savedGames)) {
				if (savedGames->savedGames.Num() >= 10) {
					savedGames->savedGames.RemoveAt(0);
					savedGames->savedGames.Add(gameBoard->SaveCurrentMap());
				}
				else {
					savedGames->savedGames.Add(gameBoard->SaveCurrentMap());
				}
				// save the game back to save slot
				UGameplayStatics::SaveGameToSlot(savedGames, FString(TEXT("SaveSlot")), 0);
			}
		}
	}
}

void AFPPlayerController::FindChatUI()
{
	// when UI is created, try finding a chat manager
	UWorld* world = GetWorld();
	if (ensure(world)) {
		TArray<AActor*> allChatMan;
		UGameplayStatics::GetAllActorsOfClass(world, AChatManager::StaticClass(), allChatMan);
		// only assign correct game board if there is one found
		if (allChatMan.Num() > 0) {
			UE_LOG(LogTemp, Warning, TEXT("Player found chatmanager"));
			AChatManager* tempChatManager = Cast<AChatManager>(allChatMan[0]);
			if (ensure(inGameUI)) {
				inGameUI->BindChatManagerWithUI(tempChatManager);
				inGameUI->RelateChatUIWithPlayer(this);
			}
			// could also be a lobby UI if a inGameUI is not created
			if (lobbyUI) {
				lobbyUI->BindChatManagerWithUI(tempChatManager);
				lobbyUI->RelateChatUIWithPlayer(this);
			}
		}
	}
}

void AFPPlayerController::FindCrewManager()
{
	// when the game starts, try to find a crew manager
	UWorld* world = GetWorld();
	if (ensure(world)) {
		TArray<AActor*> allCrewMan;
		UGameplayStatics::GetAllActorsOfClass(world, ACrewManager::StaticClass(), allCrewMan);
		// only assign correct game board if there is one found
		if (allCrewMan.Num() > 0) {
			UE_LOG(LogTemp, Warning, TEXT("Player found crew manger"));
			ACrewManager* tempCrewManager = Cast<ACrewManager>(allCrewMan[0]);
			if (ensure(tempCrewManager)) {
				crewMan = tempCrewManager;
				if (IsLocalPlayerController()) {
					crewMan->AssociatePlayer();
				}
			}
		}
	}
}

void AFPPlayerController::JoinGameLobby()
{
	if (!ensure(lobbyMan)) return;
	if (!ensure(crewMan)) return;
	if (IsLocalController()) {
		UE_LOG(LogTemp, Warning, TEXT("Player in lobby"));
		// relate both of these managers
		lobbyMan->SetCrewManager(crewMan);
		crewMan->SetLobbyManager(lobbyMan);
		// Now create a lobby UI since we are in lobby
		lobbyUI = CreateWidget<ULobbyUI>(this, LobbyUIClass);
		if (ensure(lobbyUI)) {
			// binding the lobby UI and add it to viewport
			lobbyMan->BindLobbyUI(lobbyUI);
			lobbyUI->BindCrewManagerWithUI(crewMan);
			lobbyUI->BindLobbyManagerWithUI(lobbyMan);
			lobbyUI->AddToViewport();
			lobbyMan->ShowLobbyInfo();
			// for the server side, since its host, change button text
			if (HasAuthority()) {
				lobbyUI->ChangeJoinStartButtonStatus("Start", false);
			}
			else {
				lobbyUI->ChangeJoinStartButtonStatus("Ready", true);
			}
			// bind alseo the firefighter with this UI
			AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
			if (ensure(fireFighterPawn)) {
				fireFighterPawn->BindLobbyUIFirefighter(lobbyUI);
				// join the lobby since we are local player
				ServerJoinLobby(lobbyMan, fireFighterPawn, playerName);
			}
		}
	}
}

void AFPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Player name is: %s"), *playerName);
	FindGameBoard();
	// if not in game board, we are in lobby
	if (!gameBoard) {
		FindLobbyManager();
	}
	FindCrewManager();
	// it could also be that we found both lobby manager and crew manager in a lobby
	if (lobbyMan && crewMan) {
		JoinGameLobby();
	}
	// TODO on later version make different UI with regard of different game
	MakeBasicFireFighterUI();
	MakeOptionPromptUI();
	FindChatUI();
	
	// if found a game board, switch role with regard to currently selected role
	if (gameBoard) {
		UFlashPointGameInstance* gameInst = Cast<UFlashPointGameInstance>(GetGameInstance());
		if (ensure(gameInst)) {
			ERoleType tempRole = gameInst->GetSelectedRole();
			if (tempRole != ERoleType::Basic) {
				EnableAPConsumption(false);
				SwitchRole(tempRole);
				EnableAPConsumption(true);
				EnableOperations(false);
			}
		}
	}

	if (ensure(inGameUI)) {
		inGameUI->NotifyPlaceFirefighter();
		// if both crew manager and lobby manager found, in lobby we are, create a lobby UI
		inGameUI->EnableOperationPanels(false);
	}
}

void AFPPlayerController::SetOpenDoor()
{
	CurrentOperation = EGameOperations::OpenDoor;
}

void AFPPlayerController::SetNone()
{
	CurrentOperation = EGameOperations::None;
}

void AFPPlayerController::SetPlaceFireFighter()
{
	CurrentOperation = EGameOperations::PlaceFireFighter;
}

void AFPPlayerController::SetRespawnFireFighter()
{
	CurrentOperation = EGameOperations::RespawnFireFighter;
}

void AFPPlayerController::SetMove()
{
	CurrentOperation = EGameOperations::Move;
}

void AFPPlayerController::SetExtinguishFire()
{
	CurrentOperation = EGameOperations::ExtinguishFire;
	CurrentOperation = EGameOperations::ExtinguishFire;
}

void AFPPlayerController::SetChopWall()
{
	CurrentOperation = EGameOperations::ChopWall;
}

void AFPPlayerController::SetCarry()
{
	CurrentOperation = EGameOperations::Carry;
}

void AFPPlayerController::SetFireDeckGun()
{
	CurrentOperation = EGameOperations::FireDeckGun;
}

void AFPPlayerController::SetFlip()
{
	CurrentOperation = EGameOperations::FlipPOI;
}

void AFPPlayerController::SetDriveAmbulance()
{
	CurrentOperation = EGameOperations::DriveAmbulance;
	inGameUI->EnableOperationPanels(false);
}

void AFPPlayerController::SetRadio()
{
	if (CurrentOperation != EGameOperations::DriveAmbulance && CurrentOperation != EGameOperations::DriveFireEngine && CurrentOperation != EGameOperations::GetOutAmbulance && CurrentOperation != EGameOperations::GetOutFireEngine)
	{
		CurrentOperation = EGameOperations::Radio;
	}
}

bool AFPPlayerController::SetGetOutAmbulance()
{
	if (CurrentOperation == EGameOperations::DriveAmbulance)
	{
		CurrentOperation = EGameOperations::GetOutAmbulance;
		return true;
	}
	return false;
}

bool AFPPlayerController::SetGetOutFireEngine()
{
	if (CurrentOperation == EGameOperations::DriveFireEngine)
	{
		CurrentOperation = EGameOperations::GetOutFireEngine;
		return true;
	}
	return false;
}

void AFPPlayerController::SetDriveFireEngine()
{
	CurrentOperation = EGameOperations::DriveFireEngine;
	inGameUI->EnableOperationPanels(false);
}

void AFPPlayerController::SetDodge()
{
	CurrentOperation = EGameOperations::Dodge;
}

void AFPPlayerController::SetSqueeze()
{
	CurrentOperation = EGameOperations::Squeeze;
}

void AFPPlayerController::SetCommand()
{
	CurrentOperation = EGameOperations::Command;
}

void AFPPlayerController::CancelCommand()
{
	if (ensure(commandedPawn)) {
		// uncommand the pawn
		commandedPawn->SetIsCommanded(false);
		commandedPawn->ShowIsCommanded(false);
		commandedPawn = nullptr;
		// enable the widgets
		EnableOperations(true);
		SetNone();
	}
}

void AFPPlayerController::NotifyCarryVictim(bool isCarrying)
{
	if (ensure(inGameUI)) {
		inGameUI->ShowCarrying(isCarrying);
	}
}

void AFPPlayerController::NotifyLeadVictim(bool isCarrying)
{
	if (ensure(inGameUI)) {
		inGameUI->ShowLeading(isCarrying);
	}
}

void AFPPlayerController::NotifyCarryHazmat(bool isCarrying)
{
	if (ensure(inGameUI)) {
		inGameUI->ShowCarryHazmat(isCarrying);
	}
}

void AFPPlayerController::SwitchRole(ERoleType inRole)
{
	// get the localplayer's firefighter
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (!ensure(fireFighterPawn))	return;
	// Switch role widget first
	SwitchRoleWidget(inRole);
	// if it is our ture, enable the widgets
	if (ensure(gameBoard)) {
		if (myTurnNum == gameBoard->GetCurrentTurn()) {
			if (ensure(inGameUI)) {
				// if this is not start of the game, do not show the place firefighter UI
				inGameUI->EnableOperationWidgets();
			}
		}
	}
	// just switch fire fighter role if the player is server
	if (HasAuthority()) {
		if (ensure(crewMan)) {
			crewMan->SwitchRolesFromTo(fireFighterPawn->GetFireFighterRole(), inRole);
			fireFighterPawn->SetFireFighterRole(inRole);
			fireFighterPawn->AdjustRoleProperties(inRole);
			fireFighterPawn->AdjustFireFighterAP(-fireFighterPawn->GetCrewChangeConsumption());
		}
	}
	else {
		// Otherwise call server to switch role
		ServerSwitchRole(crewMan, fireFighterPawn, inRole);
		fireFighterPawn->AdjustFireFighterAP(-fireFighterPawn->GetCrewChangeConsumption());
	}
}

void AFPPlayerController::SelectRole(ERoleType inRole, AFireFighterPawn* inPawn)
{
	// TODO some UI updates are needed here
	UFlashPointGameInstance* gameInst = Cast<UFlashPointGameInstance>(GetGameInstance());
	if (ensure(gameInst)) {
		if (ensure(lobbyMan)) {
			ServerSelectRole(inPawn, lobbyMan, inRole);
		}
	}
}

void AFPPlayerController::SwitchRoleWidget(ERoleType inRole)
{
	// remove the old widget from view port, and add a new one
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (!ensure(fireFighterPawn))	return;
	if (!ensure(inGameUI)) return;
	if (inRole == ERoleType::Basic) return;
	inGameUI->RemoveFromViewport();
	switch (inRole)
	{
	case ERoleType::Basic:
		// Impossible to be here, do nothing...
		break;
	case ERoleType::Paramedic:
		if (ensure(ParamedicClass)) {
			inGameUI = CreateWidget<UFireFighterUI>(this, ParamedicClass);
		}
		break;
	case ERoleType::FireCaptain:
		if (ensure(CaptainClass)) {
			inGameUI = CreateWidget<UFireFighterUI>(this, CaptainClass);
		}
		break;
	case ERoleType::ImagingTechnician:
		if (ensure(ImageTecClass)) {
			inGameUI = CreateWidget<UFireFighterUI>(this, ImageTecClass);
		}
		break;
	case ERoleType::CAFSFirefighter:
		if (ensure(CAFSFireFighterClass)) {
			inGameUI = CreateWidget<UFireFighterUI>(this, CAFSFireFighterClass);
		}
		break;
	case ERoleType::HazmatTechnician:
		if (ensure(HazTecClass)) {
			inGameUI = CreateWidget<UFireFighterUI>(this, HazTecClass);
		}
		break;
	case ERoleType::Generalist:
		if (ensure(GeneralistClass)) {
			inGameUI = CreateWidget<UFireFighterUI>(this, GeneralistClass);
		}
		break;
	case ERoleType::RescueSpecialist:
		if (ensure(RescueSpecClass)) {
			inGameUI = CreateWidget<UFireFighterUI>(this, RescueSpecClass);
		}
		break;
	case ERoleType::Driver:
		if (ensure(DriverClass)) {
			inGameUI = CreateWidget<UFireFighterUI>(this, DriverClass);
		}
		break;
	case ERoleType::Veteran:
		if (ensure(VeteranClass)) {
			inGameUI = CreateWidget<UFireFighterUI>(this, VeteranClass);
		}
		break;
	case ERoleType::RescueDog:
		if (ensure(RescueDogClass)) {
			inGameUI = CreateWidget<UFireFighterUI>(this, RescueDogClass);
		}
		break;
	default:
		// impossible
		break;
	}
	RelateInGameUI(fireFighterPawn);
	fireFighterPawn->RelateAllFireFighterStatus();
	FindChatUI();
	if (ensure(inGameUI)) {
		inGameUI->AddToViewport();
	}
}

EGameOperations AFPPlayerController::GetCurrentOperation()
{
	return CurrentOperation;
}

void AFPPlayerController::ServerFlashOver_Implementation(AGameBoard * board)
{
	board->FlashOverOnBoard();
}

bool AFPPlayerController::ServerFlashOver_Validate(AGameBoard * board)
{
	return true;
}

void AFPPlayerController::ServerSolveKnockDown_Implementation(AGameBoard * board)
{
	if (ensure(board))
	{
		board->ResolveKnockDownOnBoard();
	}
}

bool AFPPlayerController::ServerSolveKnockDown_Validate(AGameBoard * board)
{
	return true;
}

void AFPPlayerController::ServerAdjustCommandAP_Implementation(AFireFighterPawn * captain, int32 adjustAP)
{
	if (ensure(captain)) {
		if (captain->GetFireFighterRole() == ERoleType::FireCaptain) {
			UE_LOG(LogTemp, Warning, TEXT("adjustAP is %d"), adjustAP);
			captain->SetCommandAP(FMath::Max(captain->GetCommandAP() + adjustAP, 0));
		}
	}
}

bool AFPPlayerController::ServerAdjustCommandAP_Validate(AFireFighterPawn * captain, int32 adjustAP)
{
	return true;
}

bool AFPPlayerController::ServerHealVictim_Validate(AFireFighterPawn * fireFighterPawn, AVictim* targetVictim)
{
	return true;
}

void AFPPlayerController::ServerHealVictim_Implementation(AFireFighterPawn * fireFighterPawn, AVictim* targetVictim)
{
	UE_LOG(LogTemp, Warning, TEXT("heal victim sever"));
	if (ensure(targetVictim) && ensure(!targetVictim->IsHealed()))
	{
		targetVictim->SetHealedAndShowEffect(true);
	}
}

void AFPPlayerController::ServerSelectRole_Implementation(AFireFighterPawn* inPawn, ALobbyManager * inMan, ERoleType toRole)
{
	if (ensure(inMan)) {
		inMan->LobbyManagerSelectRole(inPawn, toRole);
		// for authority we check if it is the local pawn
		UWorld* world = GetWorld();
		if (!ensure(world)) return;
		AFPPlayerController* localPlayer = Cast<AFPPlayerController>(world->GetFirstPlayerController());
		if (localPlayer->GetPawn() == inPawn) {

		}
	}
}

bool AFPPlayerController::ServerSelectRole_Validate(AFireFighterPawn* inPawn, ALobbyManager * inMan, ERoleType toRole)
{
	return true;
}

void AFPPlayerController::ServerSolveHazmatExplosions_Implementation(AGameBoard * board)
{
	if (ensure(board))
	{
		board->ResolveHazmatExplosionOnBoard();
	}
}

bool AFPPlayerController::ServerSolveHazmatExplosions_Validate(AGameBoard * board)
{
	return true;
}
