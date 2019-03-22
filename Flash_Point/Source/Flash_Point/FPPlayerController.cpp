// Fill out your copyright notice in the Description page of Project Settings.

#include "FPPlayerController.h"
#include "Victim.h"
#include "Hazmat.h"
#include "Tile.h"
#include "Wall.h"
#include "Door.h"
#include "GameBoard.h"
#include "ChatManager.h"

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
	if (ensure(inGameUI)) {
		inGameUI->NotifyTurnBegin();
	}
}

void AFPPlayerController::EndPlayerTurn()
{
	// restore AP
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn)) {
		int32 currentAP = fireFighterPawn->GetCurrentAP();
		int32 apToRestore = 0;
		if (currentAP + fireFighterPawn->GetRestoreAP() > fireFighterPawn->GetMaxAP()) {
			apToRestore = fireFighterPawn->GetMaxAP() - currentAP;
		}
		else {
			apToRestore = fireFighterPawn->GetRestoreAP();
		}
		// if ap to restore is more than 0 restore AP
		if (apToRestore > 0) {
			fireFighterPawn->AdjustFireFighterAP(apToRestore);
		}
	}
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
	if (ensure(currentTile))
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
		fireFighterPawn->SetVictim(nullptr);
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
}

bool AFPPlayerController::ServerDropVictim_Validate(AFireFighterPawn * fireFighterPawn)
{
	return true;
}

void AFPPlayerController::ServerCarryVictim_Implementation(AFireFighterPawn * fireFighterPawn)
{
	ATile* currentTile = fireFighterPawn->GetPlacedOn();
	if (ensure(currentTile))
	{
		if (currentTile->GetPOIStatus() == EPOIStatus::Revealed)
		{
			if (currentTile->GetVictims()->Num() > 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Before pop(). Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());
				AVictim* carriedVictim = currentTile->GetVictims()->Pop(true);
				if (!ensure(carriedVictim)) return;
				carriedVictim->SetIsCarried(true);
				UE_LOG(LogTemp, Warning, TEXT("After pop(). Current Tile: %s have %d victims."), *currentTile->GetName(), currentTile->GetVictims()->Num());
				fireFighterPawn->SetVictim(carriedVictim);
				carriedVictim->victimMesh->SetVisibility(false);
				if (currentTile->GetVictims()->Num() == 0)
				{
					currentTile->SetPOIStatus(EPOIStatus::Empty);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No victim."));
			}
		}
	}
}

bool AFPPlayerController::ServerCarryVictim_Validate(AFireFighterPawn * fireFighterPawn)
{
	return true;
}

void AFPPlayerController::ServerCarryHazmat_Implementation(AFireFighterPawn * fireFighterPawn)
{
	ATile* currentTile = fireFighterPawn->GetPlacedOn();
	if (ensure(currentTile))
	{
		AHazmat* carriedHazmat = currentTile->GetHazmat();
		if (carriedHazmat)
		{
			carriedHazmat->isCarried = true;
			fireFighterPawn->SetHazmat(carriedHazmat);
			carriedHazmat->hazmatMesh->SetVisibility(false);
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

void AFPPlayerController::ServerSwitchRole_Implementation(AGameBoard * board, AFireFighterPawn * fireFighterPawn, ERoleType inRole)
{
	if (ensure(board) && ensure(fireFighterPawn)) {
		board->SwitchRolesFromTo(fireFighterPawn->GetFireFighterRole(), inRole);
		fireFighterPawn->SetFireFighterRole(inRole);
		fireFighterPawn->AdjustRoleProperties(inRole);
		fireFighterPawn->AdjustFireFighterAP(-fireFighterPawn->GetCrewChangeConsumption());
	}
}

bool AFPPlayerController::ServerSwitchRole_Validate(AGameBoard * board, AFireFighterPawn * fireFighterPawn, ERoleType inRole)
{
	return true;
}

void AFPPlayerController::DropVictim()
{
	UE_LOG(LogTemp, Warning, TEXT("Drop victim."));
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
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
			AVictim* tempVictim = fireFighterPawn->GetVictim();
			if (!ensure(tempVictim)) return;
			tempVictim->victimMesh->SetRelativeLocation(VictimSocketLocation);
		}
		ServerDropVictim(fireFighterPawn);
		// only for authority, update the firefighter's carrying UI
		if (HasAuthority()) {
			if (ensure(inGameUI)) {
				if (fireFighterPawn->GetVictim()) {
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
				if (fireFighterPawn->GetVictim()) {
					inGameUI->ShowCarrying(true);
				}
				else {
					inGameUI->ShowCarrying(false);
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
		if (ensure(fireFighterPawn->GetFireFighterRole() != ERoleType::HazmatTechnician)) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Drop hazmat."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Hazmat Technician."));
		}
	}
}

void AFPPlayerController::CarryHazmat()
{
	// TODO carry hazmat on tile when called
	UE_LOG(LogTemp, Warning, TEXT("Carry hazmat."));
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
		if (!(fireFighterPawn->GetFireFighterRole() != ERoleType::HazmatTechnician)) { return; }
	}
}

void AFPPlayerController::RevealPOI(ATile* targetTile)
{
	UE_LOG(LogTemp, Warning, TEXT("Reveal POI."));
	AFireFighterPawn* fireFighterPawn = Cast<AFireFighterPawn>(GetPawn());
	if (ensure(fireFighterPawn))
	{
		if (ensure(fireFighterPawn->GetFireFighterRole() == ERoleType::ImagingTechnician))
		{
			if (ensure(targetTile))
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

				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No POI to reveal"));
				}
			}
		}
	}
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

void AFPPlayerController::FindChatUI()
{
	// when UI is created, try finding a chat manager
	UWorld* world = GetWorld();
	if (ensure(world)) {
		TArray<AActor*> allChatMan;
		UGameplayStatics::GetAllActorsOfClass(world, AChatManager::StaticClass(), allChatMan);
		// only assign correct game board if there is one found
		if (allChatMan.Num() > 0) {
			UE_LOG(LogTemp, Warning, TEXT("Player found gameboard"));
			AChatManager* tempChatManager = Cast<AChatManager>(allChatMan[0]);
			if (ensure(inGameUI)) {
				inGameUI->BindChatManagerWithUI(tempChatManager);
				inGameUI->RelateChatUIWithPlayer(this);
			}
		}
	}
}

void AFPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Player GIAO!"));
	FindGameBoard();
	// TODO on later version make different UI with regard of different game
	MakeBasicFireFighterUI();
	FindChatUI();
	if (ensure(inGameUI)) {
		inGameUI->NotifyPlaceFirefighter();
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
}

void AFPPlayerController::SetChopWall()
{
	CurrentOperation = EGameOperations::ChopWall;
}

void AFPPlayerController::SetCarry()
{
	CurrentOperation = EGameOperations::Carry;
}

void AFPPlayerController::NotifyCarryVictim(bool isCarrying)
{
	if (ensure(inGameUI)) {
		inGameUI->ShowCarrying(isCarrying);
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
		if (ensure(gameBoard)) {
			gameBoard->SwitchRolesFromTo(fireFighterPawn->GetFireFighterRole(), inRole);
			fireFighterPawn->SetFireFighterRole(inRole);
			fireFighterPawn->AdjustRoleProperties(inRole);
			fireFighterPawn->AdjustFireFighterAP(-fireFighterPawn->GetCrewChangeConsumption());
		}
	}
	else {
		// Otherwise call server to switch role
		ServerSwitchRole(gameBoard, fireFighterPawn, inRole);
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