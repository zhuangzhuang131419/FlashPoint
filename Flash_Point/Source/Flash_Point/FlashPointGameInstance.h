// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FlashPointSaveGame.h"
#include "GeneralTypes.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "FlashPointGameInstance.generated.h"

class UMainMenu;
class FOnlineSessionSearch;

/**
 * 
 */

 // a session name used by all classes
const static FName SESSION_NAME = TEXT("FpfrTova");
// a key for decrepeting the lobby infor passed with the session
const static FName SESSION_INFO_KEY = TEXT("DcRPTFpfRTOva");
const static int32 FPSESSION_STANDARD_SIZE = 6;
const static int32 MAX_SESSION_SEARCH = 200;

UCLASS()
class FLASH_POINT_API UFlashPointGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	virtual void Init() override;
	
	// A function to produce a random indicator from the randomIndicators
	UFUNCTION(BlueprintCallable, Category = "Map Generation")
	FSpawnIndicator GetRandomMap();
	UFUNCTION(BlueprintCallable, Category = "Map Generation")
	FSpawnIndicator GetSpecificMap(int32 index);
	// getter and setter for game type
	UFUNCTION(BlueprintCallable, Category = "Map Generation")
	EGameType GetGameType();
	UFUNCTION(Exec, BlueprintCallable, Category = "Map Generation")
	void SetGameType(EGameType inGameType);
	// getter and setter for selected role
	UFUNCTION(BlueprintCallable, Category = "Map Generation")
	ERoleType GetSelectedRole();
	UFUNCTION(Exec, BlueprintCallable, Category = "Map Generation")
	void SetSelectedRole(ERoleType inRoleType);
	// Setting the game to be loaded
	UFUNCTION(BlueprintCallable, Category = "Map Generation")
	int32 GetLoadGameIndex();
	UFUNCTION(Exec, BlueprintCallable, Category = "Map Generation")
	void SetLoadGameIndex(int32 index);
	// load a certain map
	UFUNCTION(Exec, BlueprintCallable, Category = "Map Generation")
	void LoadMapOfIndex(int32 index);
	// Setting the game to be loaded
	UFUNCTION(Exec, BlueprintCallable, Category = "Map Generation")
	FMapSaveInfo GetLoadedGame();
	// creating a lobby with this function	
	UFUNCTION(BlueprintCallable, Category = "Map Generation")
	void CreateGameLobby(FGameLobbyInfo inLobbyInfo);
	UFUNCTION(BlueprintCallable, Category = "Map Generation")
	FGameLobbyInfo GetLobbyInfo() { return lobbyInfo; }
	UFUNCTION(BlueprintCallable, Category = "Lobby Creation")
	FString GetTravelURLFromLobbyInfo(FGameLobbyInfo inInfo);
	// A function to associate the game instance with a menu UI
	UFUNCTION(BlueprintCallable, Category = "Lobby Creation")
	void AssociateMenuUI(UMainMenu* inUI);
	// A function to join the session of specified index
	void JoinSessionOfIndex(int32 index);
	// A function to refresh lobby list
	void RefreshLobbyList();

protected:
	// FIELDS
	// randomly select a map indicator from preset
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Game Data Presets")
	TArray<FSpawnIndicator> randomIndicators;
	// a field to store the game mode the player started
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Map Generation")
	EGameType gameType = EGameType::Family;
	// The selected role by the player at lobby
	UPROPERTY(BlueprintReadWrite, Category = "Lobby Creation")
	ERoleType selectedRole = ERoleType::Basic;
	// a field to store the game mode the player started
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Map Generation")
	int32 loadGameIndex = -1;
	// store the loaded map
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Save Load Related")
	FMapSaveInfo loadedMap;
	// The game instance needs a lobby info to create the game
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Lobby Creation")
	FGameLobbyInfo lobbyInfo;
	// widgtes binded to the game isntance for finding and joining games with a valid GUI
	UPROPERTY(BlueprintReadWrite, Category = "Lobby Creation")
	UMainMenu* mainMenuUI = nullptr;

	// Session delicates
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type type);
	void CreateGameSession();

	// ONLINE SUBSYSTEM RELATED
	bool isDefaultOLSS = false;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
