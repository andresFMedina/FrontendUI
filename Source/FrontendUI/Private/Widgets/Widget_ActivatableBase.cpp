// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_ActivatableBase.h"
#include "Controllers/FrontendPlayerController.h"

AFrontendPlayerController* UWidget_ActivatableBase::GetOwningFrontendPlayerController()
{
	if (!CachedOwningPC.IsValid())
	{
		CachedOwningPC = GetOwningPlayer<AFrontendPlayerController>();
	}

	return CachedOwningPC.IsValid() ? CachedOwningPC.Get() : nullptr;
}
