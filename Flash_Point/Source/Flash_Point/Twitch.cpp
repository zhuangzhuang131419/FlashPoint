// Fill out your copyright notice in the Description page of Project Settings.
/* Codes below has no quality and are very very disgusting, please be extra cautious!!! */

#include "Twitch.h"
#include <string>


ATwitch::ATwitch()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATwitch::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

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

	FSocket* Socket = SocketSub->CreateSocket(NAME_Stream, TEXT("Socket"), false);
	if (Socket == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create socket"));
		return false;
	}

	int32 NewSize;
	Socket->SetReceiveBufferSize(1024 * 1024, NewSize);
	Socket->SetReuseAddr(true);

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
	bool k = Send("PASS " + this->Oauth);
	bool ok = Send("NICK " + this->Username);
	bool okay = Send("JOIN #" + this->Channel);

	bool imokay = k && ok && okay;
	if (!imokay)
	{
		UE_LOG(LogTemp, Error, TEXT("no ok"));
		return false;
	}
	return true;
}

bool ATwitch::Send(FString Stuff)
{
	Stuff += "\n";
	TCHAR* TStuff = Stuff.GetCharArray().GetData();

	int32 OutSent;
	return this->CurrentSocket->Send((uint8*)TCHAR_TO_UTF8(TStuff), FCString::Strlen(TStuff), OutSent);
}

bool ATwitch::ConnectAPI()
{
	if (Connect())
	{
		return Authenticate();
	}
	return false;
}

bool ATwitch::Read(FString& Out) const
{
	if (CurrentSocket == nullptr)
	{
		return false;
	}

	TArray<uint8> Shit;
	uint32 Size;
	bool ok = false;
	if (CurrentSocket->HasPendingData(Size))
	{
		ok = true;
		Shit.SetNumUninitialized(Size);
		int32 ShitReader;
		CurrentSocket->Recv(Shit.GetData(), Shit.Num(), ShitReader);
	}

	FString Stuff = "";
	if (ok)
	{
		const std::string c_string_data(reinterpret_cast<const char*>(Shit.GetData()), Shit.Num());
		Stuff = FString(c_string_data.c_str());
	}

	if (Stuff != "")
	{
		Out = Stuff;
		return true;
	}

	return false;
}