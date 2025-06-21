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
	UFUNCTION()
	FORCEINLINE float GetMusicVolume() const { return MusicVolume; }
	UFUNCTION()
	FORCEINLINE void SetMusicVolume(float InVolume) { MusicVolume = InVolume; }
	UFUNCTION()
	FORCEINLINE float GetSoundFXVolume() const { return SoundFXVolume; }
	UFUNCTION()
	FORCEINLINE void SetSoundFXVolume(float InVolume) { SoundFXVolume = InVolume; }

	UFUNCTION()
	FORCEINLINE bool IsBackgroundAudioEnabled() const { return bEnableBackgroundAudio; }

	UFUNCTION()
	FORCEINLINE void SetEnableBackgroundAudio(bool bEnable) { bEnableBackgroundAudio = bEnable; }

	UFUNCTION()
	FORCEINLINE bool IsHDRAudioEnabled() const { return UseHDRAudio; }

	UFUNCTION()
	FORCEINLINE void SetUseHDRAudio(bool bEnable) { UseHDRAudio = bEnable; }
	//// Audio

	/// Video
	UFUNCTION()
	float GetCurrentDisplayGamma() const;

	UFUNCTION()
	void SetCurrentDisplayGamma(const float InNewGamma);

private:
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	
	UPROPERTY(Config)
	float OverallVolume;

	UPROPERTY(Config)
	float MusicVolume;

	UPROPERTY(Config)
	float SoundFXVolume;

	UPROPERTY(Config)
	bool bEnableBackgroundAudio;

	UPROPERTY(Config)
	bool UseHDRAudio;
};
