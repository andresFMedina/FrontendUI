// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/MyListDataObject_String.h"
#include "ListDataObject_StringResolution.generated.h"

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UListDataObject_StringResolution : public UMyListDataObject_String
{
	GENERATED_BODY()
	
public:
	void InitResolutionValues();

protected:
	virtual void OnDataObjectInitialized() override;

	virtual bool CanSetToForcedStringValue(const FString& InForcedValue) const override;
	virtual void OnSetToForcedStringValue(const FString& InForcedValue) override;

private:
	FString ResToValueString(const FIntPoint& Resolution) const;
	FText ResToDisplayText(const FIntPoint& Resolution) const;

	FString MaximumAllowedResolution;

public:
	FORCEINLINE FString GetMaximumAllowedResolution() const { return MaximumAllowedResolution; }
	
};
