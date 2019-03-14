// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatManager.h"
#include "MenuSystem/ChatWidget.h"


// Sets default values
AChatManager::AChatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AChatManager::SetChatWidget(UChatWidget * inChatUI)
{
	chatUI = inChatUI;
}

void AChatManager::SendGlobalText(FString inMessage)
{
	
	if (chatMessages.Num() > maxAllowedLines) {
		// if too many messages now, shrink the array and then append
		chatMessages.RemoveAt(0);
	}
	chatMessages.Add(inMessage);
	
	// for a server, it will always print all text out actively
	if (HasAuthority()) {
		if (ensure(chatUI)) {
			chatUI->DisplayMessages(chatMessages);
		}
	}
}

// Called when the game starts or when spawned
void AChatManager::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		SetReplicates(true);
	}
	
}

void AChatManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AChatManager, chatMessages);
}

void AChatManager::Rep_ChatMessage()
{
	if (ensure(chatUI)) {
		chatUI->DisplayMessages(chatMessages);
	}
}

// Called every frame
void AChatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

