// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath)
	: CachedDynamicFunctionPath(InSetterOrGetterFuncPath)
{
	CachedUserSettings = UFrontendGameUserSettings::Get();
}

FString FOptionsDataInteractionHelper::GetValueAsString() const
{
	FString OutStringValue;
	PropertyPathHelpers::GetPropertyValueAsString(CachedUserSettings.Get(), CachedDynamicFunctionPath, OutStringValue);

	return OutStringValue;
}

void FOptionsDataInteractionHelper::SetValueFromString(const FString& InValue)
{
	PropertyPathHelpers::SetPropertyValueFromString(CachedUserSettings.Get(), CachedDynamicFunctionPath, InValue);
}
