// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealNetwork.h"
#include "ChatManager.generated.h"

UCLASS()
class FLASH_POINT_API AChatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChatManager();

	// Setter for the binded chating widget
	UFUNCTION(BlueprintCallable, Category = "Message Chat")
	void SetChatWidget(class UChatWidget* inChatUI);
	UFUNCTION(BlueprintCallable, Category = "Message Chat")
	void SendGlobalText(FString inMessage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// FIELDS
	UPROPERTY(ReplicatedUsing = Rep_ChatMessage, VisibleAnyWhere, BlueprintReadWrite, Category = "Message Chat")
	TArray<FString> chatMessages;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Message Chat")
	UChatWidget* chatUI = nullptr;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Message Chat")
	int32 maxAllowedLines = 30;

	// Replication functions
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	UFUNCTION()
	void Rep_ChatMessage();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
