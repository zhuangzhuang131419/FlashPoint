// Fill out your copyright notice in the Description page of Project Settings.

#include "FlashPointGameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"

void UFlashPointGameInstance::Init() {
	Super::Init();
	IOnlineSubsystem* OLSS = IOnlineSubsystem::Get();
	FString OLSSName = OLSS->GetSubsystemName().ToString();
	UE_LOG(LogTemp, Warning, TEXT("Found OLSS: %s"), *OLSSName);
	if (OLSSName == "NULL") {
		// if the OnlineSubsystem is a NULL subsystem, set using default OLSS
		isDefaultOLSS = true;
		UE_LOG(LogTemp, Warning, TEXT("Using default OLSS"));
	}
	if (OLSS) {
		SessionInterface = OLSS->GetSessionInterface();
		// bind online subsystem delicate functions
		if (SessionInterface.IsValid()) {
			UE_LOG(LogTemp, Warning, TEXT("Got valid session interface"));
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UFlashPointGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UFlashPointGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UFlashPointGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UFlashPointGameInstance::OnJoinSessionComplete);
		}
	}
}

FSpawnIndicator UFlashPointGameInstance::GetRandomMap() {
	// randomize a integer to return as random map's index
	if (randomIndicators.Num() > 0) {
		int32 randNum = FMath::RandRange(0, randomIndicators.Num() - 1);
		return randomIndicators[randNum];
	}
	else {
		return FSpawnIndicator();
	}
}

FSpawnIndicator UFlashPointGameInstance::GetSpecificMap(int32 index)
{
	if (index < 0 || index >= randomIndicators.Num()) {
		return FSpawnIndicator();
	}
	else {
		return randomIndicators[index];
	}
}

EGameType UFlashPointGameInstance::GetGameType()
{
	return gameType;
}

void UFlashPointGameInstance::SetGameType(EGameType inGameType)
{
	gameType = inGameType;
}

ERoleType UFlashPointGameInstance::GetSelectedRole()
{
	return selectedRole;
}

void UFlashPointGameInstance::SetSelectedRole(ERoleType inRoleType)
{
	selectedRole = inRoleType;
}

int32 UFlashPointGameInstance::GetLoadGameIndex()
{
	return loadGameIndex;
}

void UFlashPointGameInstance::SetLoadGameIndex(int32 index)
{
	loadGameIndex = index;
}

void UFlashPointGameInstance::LoadMapOfIndex(int32 index)
{
	if (UGameplayStatics::DoesSaveGameExist(FString(TEXT("SaveSlot")), 0)) {
		UFlashPointSaveGame* savedGames = Cast<UFlashPointSaveGame>(UGameplayStatics::LoadGameFromSlot(FString(TEXT("SaveSlot")), 0));
		if (ensure(savedGames)) {
			// Get the saved games
			if (savedGames->savedGames.Num() > index) {
				loadedMap = savedGames->savedGames[index];	
				gameType = loadedMap.boardInfo.gameModeType;
				UE_LOG(LogTemp, Warning, TEXT("Loaded map"));
			}
		}
	}
}

FMapSaveInfo UFlashPointGameInstance::GetLoadedGame()
{
	return loadedMap;
}

void UFlashPointGameInstance::CreateGameSession()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating game session"));
	// create a session
	if (SessionInterface.IsValid()) {
		// depending on the session interface, create either a lan game or a net game
		FOnlineSessionSettings sessionSetting;
		if (isDefaultOLSS) {
			// adjust session settings
			sessionSetting.bIsLANMatch = true;

		}
		else {
			// adjust session settings
			sessionSetting.bIsLANMatch = false;
		}
		sessionSetting.NumPublicConnections = FPSESSION_STANDARD_SIZE;
		sessionSetting.bShouldAdvertise = true;
		sessionSetting.bUsesPresence = true;
		// set the session with lobby infos
		sessionSetting.Set(SESSION_INFO_KEY, lobbyInfo.Encrypt(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->CreateSession(0, SESSION_NAME, sessionSetting);
	}
}

void UFlashPointGameInstance::CreateGameLobby(FGameLobbyInfo inLobbyInfo)
{
	FString encrypted = inLobbyInfo.Encrypt();
	FGameLobbyInfo decrypted = FGameLobbyInfo::DecryptLobbyInfo(encrypted);
	UE_LOG(LogTemp, Warning, TEXT("The encrypted lobby info is: %s"), *encrypted);
	UE_LOG(LogTemp, Warning, TEXT("The lobby's name is: %s"), *decrypted.lobbyName);
	UE_LOG(LogTemp, Warning, TEXT("The lobby's health is: %d"), decrypted.boardHealth);
	
	// first set the lobby info to the game instance
	lobbyInfo = inLobbyInfo;

	// create a session if there isn't one already
	if (SessionInterface.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("Session valid"));
		FNamedOnlineSession* existSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (existSession) {
			UE_LOG(LogTemp, Warning, TEXT("Session already exists"));
			// if the session already exist, destroy the session
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Session first created"));
			CreateGameSession();
		}
	}
}

FString UFlashPointGameInstance::GetTravelURLFromLobbyInfo(FGameLobbyInfo inInfo)
{
	// TODO remember to check and replace paths at final build
	FString travelURL;
	if (inInfo.isSaved) {
		travelURL = "/Game/maps/LoadGameLevel?listen";
	}
	else {
		switch (inInfo.map)
		{
		case EGameMap::FamilyDefault:
			travelURL = "/Game/maps/TestLevel?listen";
			break;
		case EGameMap::RandomGenerate:
			travelURL = "/Game/maps/RandBoardLevel?listen";
			break;
		case EGameMap::RandomSelect:
			// TODO add random select in future
			travelURL = "/Game/maps/TestLevel?listen";
			break;
		case EGameMap::UniversityOnFire:
			travelURL = "/Game/maps/UniversityOnFire?listen";
			break;
		case EGameMap::DeansParty:
			travelURL = "/Game/maps/DeansParty?listen";
			break;
		default:
			travelURL = "/Game/maps/TestLevel?listen";
			break;
		}
	}
	return travelURL;
}

void UFlashPointGameInstance::AssociateMenuUI(UMainMenu * inUI)
{
	mainMenuUI = inUI;
}

void UFlashPointGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	// as the session gets created, travel to desired map
	if (!Success) return;
	UWorld* world = GetWorld();
	if (!ensure(world)) return;
	// travel to the lobby from here as a server
	world->ServerTravel("/Game/maps/LobbyLevel?listen");
}

void UFlashPointGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	// when the session is destroyed, do create session
	if (Success) {
		UE_LOG(LogTemp, Warning, TEXT("Session successfully destroyed"));
		CreateGameSession();
	}
}

void UFlashPointGameInstance::OnFindSessionComplete(bool Success)
{
}

void UFlashPointGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type type)
{
}

