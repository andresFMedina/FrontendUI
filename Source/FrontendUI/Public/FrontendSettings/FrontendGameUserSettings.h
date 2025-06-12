// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	static UFrontendGameUserSettings* Get();

	UFUNCTION()
	FORCEINLINE FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }
	UFUNCTION()
	FORCEINLINE void SetCurrentGameDifficulty(const FString& InGameDifficulty) { CurrentGameDifficulty = InGameDifficulty; }

private:
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	
};
