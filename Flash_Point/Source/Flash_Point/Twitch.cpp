// Fill out your copyright notice in the Description page of Project Settings.

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

