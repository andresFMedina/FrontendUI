// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendSettings/FrontendLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UFrontendLoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const
{
    checkf(!SoftLoadingScreenWidgetClass.IsNull(), TEXT("Loading widget not assigned!"))
    TSubclassOf<UUserWidget> LoadedScreenWidget = SoftLoadingScreenWidgetClass.LoadSynchronous();

    return LoadedScreenWidget;         
}
