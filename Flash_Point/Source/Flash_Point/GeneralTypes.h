// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralTypes.generated.h"

class ATile;

const static int32 ENCRYPTION_LENGTH = 8;

/**
 * 
 */
// An enum for different operations
UENUM(BlueprintType)
enum class ERoleType : uint8
{
	Basic,
	Paramedic,
	FireCaptain,
	ImagingTechnician,
	CAFSFirefighter,
	HazmatTechnician,
	Generalist,
	RescueSpecialist,
	Driver,
	Veteran,
	RescueDog,
};

// An enum for different operations
UENUM(BlueprintType)
enum class EGameOperations : uint8
{
	PlaceFireFighter,
	RespawnFireFighter,
	Move,
	ChopWall,
	ExtinguishFire,
	Carry,
	OpenDoor,
	DriveAmbulance,
	DriveFireEngine,
	GetOutAmbulance,
	GetOutFireEngine,
	PlaceAmbulance,
	PlaceFireEngine,
	Radio,
	FireDeckGun,
	FlipPOI,
	Dodge,
	Squeeze,
	Command,
	None
};

// An enum for different tile types
UENUM(BlueprintType)
enum class ETileType : uint8
{
	Default,
	FireEnginePark,
	AmbulancePark
};

// An enum for edge types
UENUM(BlueprintType)
enum class EEdgeType : uint8
{
	Empty,
	Wall,
	Door
};

// An enum for different operations
UENUM(BlueprintType)
enum class EAcceptanceStatus : uint8
{
	Accepted,
	Rejected,
	Waiting,
	Empty
};

// An enum for tile status
UENUM(BlueprintType)
enum class EFireStatus : uint8
{
	Clear,
	Smoke,
	Fire
};

// An enum for POI status
UENUM(BlueprintType)
enum class EPOIStatus : uint8
{
	Empty,
	Revealed,
	Hided
};

// An enum for direction
UENUM(BlueprintType)
enum class EDirection : uint8
{
	Up,
	Down,
	Left,
	Right
};

// An enum for game mode types
UENUM(BlueprintType)
enum class EGameType : uint8
{
	Family,
	Experienced,
	Betryal
};

// A struct indicate the option prompt
UENUM(BlueprintType)
enum class EOptionPromptType : uint8
{
	Dodge,
	FireDeckGunReRoll,
	CommandDoor,
	CommandTile,
	FireDeckGunPositionUnComfirm,
};

// An Enum indicating which game map is the game in
UENUM(BlueprintType)
enum class EGameMap : uint8
{
	FamilyDefault,
	RandomSelect,
	RandomGenerate,
	UniversityOnFire,
	DeansParty
};

// An Enum indicating the player's status in the lobby
UENUM(BlueprintType)
enum class EJoinStatus : uint8
{
	Ready,
	Waiting,
	Host,
	NotConnected
};

// A struct indicate the location
USTRUCT(BlueprintType)
struct FLocation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location info")
	int32 xLoc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location info")
	int32 yLoc;
	FLocation() : xLoc(0), yLoc(0) {}
	FLocation(int32 x, int32 y) 
	{
		xLoc = x;
		yLoc = y;
	}
};

// A struct used to indicate map generation
USTRUCT(BlueprintType)
struct FSpawnIndicator
{
	GENERATED_USTRUCT_BODY()

	// An integer array to specify the locations of door for the four boundaries in orde of front, back, left, right
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> doorLoc;
	// An integer array to specify the locations of fire engine parks in orde of front, back, left, right
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> engineParkLoc;
	// An integer array to specify the locations of fire engine parks in orde of front, back, left, right
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> ambulanceParkLoc;
	// An int array to indicate to which tile's right has a wall on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> wallRight;
	// An int array to indicate to which tile's front has a wall on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> wallFront;
	// An int array to indicate to which door's right has a wall on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> doorRight;
	// An int array to indicate to which door's front has a wall on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map info")
	TArray<int32> doorFront;
};

// A struct to store nodes for a star searching on shortest path tile
USTRUCT(BlueprintType)
struct FSearchNode
{
	GENERATED_USTRUCT_BODY()
		
	// integer for expected
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search Node")
	int32 heuristic;
	// integer for actual path cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search Node")
	int32 cost;
	// tile for node tile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search Node")
	ATile* nodeTile = nullptr;

	// override the < operator to be used in heap naking
	friend bool operator<(const FSearchNode& lhs, const FSearchNode& rhs) {
		if (lhs.heuristic < rhs.heuristic) {
			return true;
		}
		else if (lhs.heuristic == rhs.heuristic && lhs.cost > rhs.cost) {
			return true;
		}
		else {
			return false;
		}
	}

	// override the == operator to be used in heap naking
	friend bool operator==(const FSearchNode& lhs, const FSearchNode& rhs) {
		if (lhs.heuristic == rhs.heuristic && lhs.cost == rhs.cost && lhs.nodeTile == rhs.nodeTile) {
			return true;
		}
		else {
			return false;
		}
	}

	FSearchNode() : heuristic(0), cost(0), nodeTile(nullptr) {}
	FSearchNode(int32 heur, int32 nodeCost, ATile* tile) : heuristic(heur), cost(nodeCost), nodeTile(tile) {}
};

// A struct to store board save info only
USTRUCT(BlueprintType)
struct FBoardSaveInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 currentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 victimLostNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 maxLostVictim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 victimSavedNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 maxSavedVictim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 maxPOI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 falseAlarmNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 totalVictimNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 removedHazmat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	FSpawnIndicator indicator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	EGameType gameModeType;

};

// A struct to store all informations on a specific edge
USTRUCT(BlueprintType)
struct FEdgeSaveInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// if the saved game is valid (default is false)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	EEdgeType type;
	// if the edge is blocked
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	bool isBlocked = true;
	// if the wall is choped or the door is opened
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	bool chopedOrOpened = false;
	// for door only, if the door is destroyed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	bool isDestroyed = false;


};

// A struct to store all informations on a specific tile
USTRUCT(BlueprintType)
struct FTileSaveInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// if the tile has POI that's not revealed on it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	bool hasPOI = false;
	// if the tile is on fire or smoke
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	EFireStatus fireStatus = EFireStatus::Clear;
	// how many victims are on that specific tile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	int32 victimsOnTile = 0;
	// if there is a hazmat on the tile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	bool hasHazmat = false;
	// if there is a hotspot on the tile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	bool hasHotspot = false;


};

// A struct to store all informations about a saved game
USTRUCT(BlueprintType)
struct FMapSaveInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// if the saved game is valid (default is false)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	bool isValidSave = false;
	// the name of the save
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	FString saveName;
	// the time the game got saved
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	FString saveDate;
	// Infomation of the board
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	FBoardSaveInfo boardInfo;
	// Information on edges within the board
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	TArray<FEdgeSaveInfo> edgesInfo;
	// Information on border edges within the board
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	TArray<FEdgeSaveInfo> bordersInfo;
	// Information on tiles within the board
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	TArray<FTileSaveInfo> tilesInfo;
	// Which map it is
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Info")
	EGameMap map;
	
};

// A struct to store all informations about a saved game
USTRUCT(BlueprintType)
struct FGameLobbyInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// if this is a saved game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Info")
	bool isSaved = false;
	// What mode is the game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Info")
	EGameType mode = EGameType::Family;
	// Number of initial hazmats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Info")
	int32 initialHazmatNum;
	// Number of initial explosions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Info")
	int32 initialExplosions;
	// Number of lost victims
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Info")
	int32 lostVictimNum;
	// Remaining health of the game board in the game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Info")
	int32 boardHealth;
	// Which map is this game being played in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Info")
	EGameMap map = EGameMap::FamilyDefault;
	// Name of the lobby
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Info")
	FString lobbyName;

	// A Function that converts the lobby infomation to a string for easy sending across the server
	FString Encrypt() {
		// encrypt the map info into a string
		FString mapInfoString = "";
		// first index (from left to right) as the isSaved
		if (isSaved) {
			mapInfoString += "1";
		}
		else {
			mapInfoString += "0";
		}
		// second index stands for game mode
		switch (mode)
		{
		case EGameType::Family:
			mapInfoString += "0";
			break;
		case EGameType::Experienced:
			mapInfoString += "1";
			break;
		case EGameType::Betryal:
			mapInfoString += "2";
			break;
		default:
			break;
		}
		// third digit is the number of initial hazmats which never goes above 10 (1 digit)
		mapInfoString += FString::FromInt(initialHazmatNum);
		// fourth digit is the number of initial explosions which is also always 1 digit
		mapInfoString += FString::FromInt(initialExplosions);
		// fifth digit for number of lost victims (always 1 digit in valid game)
		mapInfoString += FString::FromInt(lostVictimNum);
		// sixth and seventh digits are the board health
		int32 twoDigitHealth = boardHealth % 100;	// to ensure there's only two digits
		int32 firstHealthDigit = twoDigitHealth / 10;	// first digit of the board health
		mapInfoString += FString::FromInt(firstHealthDigit);
		int32 secondHealthDigit = twoDigitHealth % 10;	// second digit of the health
		mapInfoString += FString::FromInt(secondHealthDigit);
		// eighth character for game mode
		switch (map)
		{
		case EGameMap::FamilyDefault:
			mapInfoString += "0";
			break;
		case EGameMap::RandomSelect:
			mapInfoString += "1";
			break;
		case EGameMap::RandomGenerate:
			mapInfoString += "2";
			break;
		case EGameMap::UniversityOnFire:
			mapInfoString += "3";
			break;
		case EGameMap::DeansParty:
			mapInfoString += "4";
			break;
		default:
			break;
		}
		// rest of the string are just lobby name given by the player
		mapInfoString += lobbyName;
		return mapInfoString;
	}

	// convert a string encryption to lobby info
	static FGameLobbyInfo DecryptLobbyInfo(FString encrypted) {
		FGameLobbyInfo lobbyInfo;
		// first 8 indices of the string is the encryption
		if (encrypted.IsValidIndex(ENCRYPTION_LENGTH)) {
			// lobby name is first get
			lobbyInfo.lobbyName = encrypted.RightChop(ENCRYPTION_LENGTH);
			// get the remaining infomations from the string
			FString encryptedInfo = encrypted.Left(ENCRYPTION_LENGTH);
			if (!encryptedInfo.IsNumeric()) return lobbyInfo;
			// decrypt the informations passed with the string
			lobbyInfo.isSaved = encryptedInfo.Mid(0, 1).ToBool();
			// get the mode from second number
			lobbyInfo.mode = static_cast<EGameType>(FCString::Atoi(*encryptedInfo.Mid(1, 1)));
			// get the number of initial hazmat from third number
			lobbyInfo.initialHazmatNum = FCString::Atoi(*encryptedInfo.Mid(2, 1));
			// get the number of initial explosions from the fourth number
			lobbyInfo.initialExplosions = FCString::Atoi(*encryptedInfo.Mid(3, 1));
			// get the number of lost victims from the fifith digit
			lobbyInfo.lostVictimNum = FCString::Atoi(*encryptedInfo.Mid(4, 1));
			// get the board health from the sixth and seventh digits
			lobbyInfo.boardHealth = FCString::Atoi(*encryptedInfo.Mid(5, 2));
			// the last index is the map specified
			lobbyInfo.map = static_cast<EGameMap>(FCString::Atoi(*encryptedInfo.Mid(7, 1)));
		}
		// all remaining indices are 
		return lobbyInfo;
	}
};

// A struct to store player information within a lobby
USTRUCT(BlueprintType)
struct FPlayerLobbyInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// if this is a saved game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Info")
	EJoinStatus joinStatus = EJoinStatus::NotConnected;
	// Name of the player in lobby
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Info")
	FString playerName = "";
	// The role the player chooses in the lobby
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Info")
	ERoleType chosenRole = ERoleType::Basic;

};

class FLASH_POINT_API GeneralTypes
{
public:
	GeneralTypes();

	// A method to use a star search to find the shortest path from one tile to another
	static int32 AStarShortest(int32 maxDepth, ATile* start, ATile* goal, TArray<ATile*> & trace, bool countSmoke = false);
	// A method to find the heuristic of a node to goal
	static int32 GetHeuristic(ATile* cur, ATile* goal);

	~GeneralTypes();
};
