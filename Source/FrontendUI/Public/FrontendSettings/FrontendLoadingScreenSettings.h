// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FrontendLoadingScreenSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game,DefaultConfig)
class FRONTENDUI_API UFrontendLoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

	TSubclassOf<UUserWidget> GetLoadingScreenWidgetClassChecked() const;

	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings")
	TSoftClassPtr<UUserWidget> SoftLoadingScreenWidgetClass;

	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings")
	float HoldLoadingScreenExtraSeconds = 3.f;

	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings")
	bool bShouldLoadingScreenInEditor = false;
	
};
