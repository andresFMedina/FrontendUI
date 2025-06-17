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
	UFrontendGameUserSettings();

	static UFrontendGameUserSettings* Get();

	// Gameplay
	UFUNCTION()
	FORCEINLINE FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }
	UFUNCTION()
	FORCEINLINE void SetCurrentGameDifficulty(const FString& InGameDifficulty) { CurrentGameDifficulty = InGameDifficulty; }

	//Audio
	UFUNCTION()
	FORCEINLINE float GetOverallVolume() const { return OverallVolume; }
	UFUNCTION()
	FORCEINLINE void SetOverallVolume(float InVolume);

private:
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	
	UPROPERTY(Config)
	float OverallVolume;
};
