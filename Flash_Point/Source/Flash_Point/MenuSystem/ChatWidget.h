// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "ChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class FLASH_POINT_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Message Chat")
	void BindChatManager(class AChatManager* inChatMan);
	UFUNCTION(BlueprintCallable, Category = "Message Chat")
	void SendMessage(FString message);
	UFUNCTION(BlueprintImplementableEvent, Category = "Message Chat")
	void DisplayMessages(const TArray<FString>& messages);
	void SetRelatedPlayer(class AFPPlayerController* inPlayer);

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UEditableTextBox* SendMessageBox = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Message Chat")
	AChatManager* chatMan = nullptr;
	AFPPlayerController* localPlayer = nullptr;
	
	
};
