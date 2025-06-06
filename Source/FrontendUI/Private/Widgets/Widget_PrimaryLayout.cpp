// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_PrimaryLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

#include "FrontendDebugHelper.h"

TObjectPtr<UCommonActivatableWidgetContainerBase> UWidget_PrimaryLayout::FindWidgetStackByTag(FGameplayTag InStackTag) const
{
	checkf(RegisteredWidgetStackMap.Contains(InStackTag), TEXT("Widget stack with tag '%s' not found in the registered widget stack map."), *InStackTag.ToString());

	return RegisteredWidgetStackMap.FindRef(InStackTag);
}

void UWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories = "Frontend.WidgetStack"))FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStackWidget)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStackWidget);			
		}
	}
}
