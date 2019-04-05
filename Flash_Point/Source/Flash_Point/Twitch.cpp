// Fill out your copyright notice in the Description page of Project Settings.
/* Codes below has no quality and are very very disgusting, please be extra cautious!!! */

#include "Twitch.h"


// Sets default values
ATwitch::ATwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATwitch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Setup initial Info (twitch oauth, username and channel)
bool ATwitch::SetInitialInfo(FString Oauth, FString Username, FString Channel)
{
	this->Oauth = Oauth;
	this->Username = Username;
	this->Channel = Channel;
	if (!Oauth.IsEmpty() && !Username.IsEmpty() && !Channel.IsEmpty())
	{
		return true;
	}
	return false;
}

bool ATwitch::Connect()
{
	auto SocketSub = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	auto HostAddr = SocketSub->CreateInternetAddr();
	auto HostResolveError = SocketSub->GetHostByName("irc.twitch.tv", *HostAddr);
	HostAddr->SetPort(6667);

	// Create a Socket
	FSocket* Socket = SocketSub->CreateSocket(NAME_Stream, TEXT("Socket"), false);
	if (Socket == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create socket"));
		return false;
	}

	int32 NewSize;
	Socket->SetReceiveBufferSize(1024 * 1024, NewSize);
	Socket->SetReuseAddr(true);

	// Check Connection
	if (!Socket->Connect(*HostAddr))
	{
		Socket->Close();
		SocketSub->DestroySocket(Socket);
		UE_LOG(LogTemp, Error, TEXT("Failed to connect"));
		return false;
	}

	this->CurrentSocket = Socket;
	UE_LOG(LogTemp, Warning, TEXT("Twtich is f**king connected"));

	return true;
}

bool ATwitch::Authenticate()
{

	if (CurrentSocket == nullptr || !UserInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to connect"));
		return false;
	}

	// Have no ideas what they are, just want to make sure if they are okay.
	bool k = SendIRC("PASS " + this->Oauth);
	bool ok = SendIRC("NICK " + this->Username);
	bool okay = SendIRC("JOIN #" + this->Channel);

	bool imokay = k && ok && okay;
	if (!imokay)
	{
		UE_LOG(LogTemp, Error, TEXT("no ok"));
		return false;
	}
	return true;
}

bool ATwitch::SendIRC(FString FMessage)
{
	// FString to TCHAR
	FMessage += "\n";
	TCHAR* TMessage = FMessage.GetCharArray().GetData();

	// Send commands
	int32 OutSent;
	return this->CurrentSocket->Send((uint8*)TCHAR_TO_UTF8(TMessage), FCString::Strlen(TMessage), OutSent);
}