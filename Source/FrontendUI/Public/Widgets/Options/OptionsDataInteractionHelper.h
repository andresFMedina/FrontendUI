// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"

class UFrontendGameUserSettings;
/**
 * 
 */
class FRONTENDUI_API FOptionsDataInteractionHelper
{
public:
	FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath);

	FString GetValueAsString() const;
	void SetValueFromString(const FString& InValue);

private:
	FCachedPropertyPath CachedDynamicFunctionPath;
	TWeakObjectPtr<UFrontendGameUserSettings> CachedUserSettings;
};
