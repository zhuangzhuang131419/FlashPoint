// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sockets.h"
#include "Runtime/Networking/Public/Networking.h"
#include "Twitch.generated.h"

UCLASS()
class FLASH_POINT_API ATwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// IRC sender
	bool Send(FString Stuff);
	bool Connect();
	bool Authenticate();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Oauth token for Twitch API connection
	UFUNCTION(BlueprintCallable, Category = "Setup")
	bool SetInitialInfo(const FString Oauth, const  FString Username, const FString Channel);
	// Method to connect
	UFUNCTION(BlueprintCallable, Category = "Setup")
	bool ConnectAPI();
	UFUNCTION(BlueprintCallable, Category = "Setup")
	bool GetStuff(FString& Out);
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void ReadStuff(FString In, FString& Nom, FString& Out);
	
	

private:
	// Parameters for Twtich authorization
	FString Oauth;
	FString Username;
	FString Channel;
	bool UserInitialized;

	// Parameters for connections
	FSocket* CurrentSocket;

	FTimerHandle TimerHandle;
};
