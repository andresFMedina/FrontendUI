// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendSettings/FrontendGameUserSettings.h"

UFrontendGameUserSettings::UFrontendGameUserSettings()
	:OverallVolume(1.0f),
	MusicVolume(1.0f),
	SoundFXVolume(1.0f),
	bEnableBackgroundAudio(false)
{
}

UFrontendGameUserSettings* UFrontendGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

void UFrontendGameUserSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;
}
