// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendFunctionLibrary.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"


TSoftClassPtr<UWidget_ActivatableBase> UFrontendFunctionLibrary::GetFrontendWidgetClassByTag(UPARAM(meta = (Categories = "Frontend.Widget"))  FGameplayTag InWidgetTag)
{
	const auto FrontendDeveloperSettings =  GetDefault<UFrontendDeveloperSettings>();

	checkf(FrontendDeveloperSettings->FrontendWidgetMap.Contains(InWidgetTag), TEXT("Could not find widget in tag %s"), *InWidgetTag.ToString());
	
	return FrontendDeveloperSettings->FrontendWidgetMap.FindRef(InWidgetTag);
}

TSoftObjectPtr<UTexture2D> UFrontendFunctionLibrary::GetOptionSoftImageByTag(FGameplayTag InImageTag)
{
	const auto FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();

	checkf(FrontendDeveloperSettings->FrontendWidgetImageMap.Contains(InImageTag), TEXT("Could not find image in tag %s"), *InImageTag.ToString());

	return FrontendDeveloperSettings->FrontendWidgetImageMap.FindRef(InImageTag);
}
