// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Value.h"
#include "MyListDataObject_String.generated.h"

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UMyListDataObject_String : public UListDataObject_Value
{
	GENERATED_BODY()

public:
	void AddDynamicOption(const FString& InOption, const FText& InDisplayName);
	void AdvanceToNextOption();
	void BackToPreviousOption();
	void OnRotatorInitiatedValueChange(const FText& InNewSelectedText);

	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;

protected:

	virtual void OnDataObjectInitialized() override;
	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);	

	TArray<FString> AvailableOptions;
	TArray<FText> AvailableOptionsDisplayNames;
	FString CurrentStringValue;
	FText CurrentDisplayName;

public:
	FORCEINLINE const TArray<FText>& GetAvailableOptionsDisplayNames() const { return AvailableOptionsDisplayNames; }
	FORCEINLINE const TArray<FString>& GetAvailableOptions() const { return AvailableOptions; }
	FORCEINLINE const FText& GetCurrentDisplayName() const { return CurrentDisplayName; }


	
};
