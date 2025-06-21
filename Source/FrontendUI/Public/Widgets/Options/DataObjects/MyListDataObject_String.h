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

UCLASS()
class FRONTENDUI_API UListDataObject_StringBool : public UMyListDataObject_String
{
	GENERATED_BODY()

public:
	void OverrideTrueDisplayText(const FText& InTrueDisplayText);
	void OverrideFalseDisplayText(const FText& InFalseDisplayText);
	void SetTrueAsDefaultValue();
	void SetFalseAsDefaultValue();

protected:
	virtual void OnDataObjectInitialized() override;

private:
	void TryInitBoolValues();

	const FString TrueString = TEXT("true");
	const FString FalseString = TEXT("false");


};

UCLASS()
class FRONTENDUI_API UListDataObject_StringEnum : public UMyListDataObject_String
{
	GENERATED_BODY()

public:
	template<typename TEnum>
	void AddEnumOption(const TEnum& InEnumValue, const FText& InDisplayText)
	{
		const UEnum* StaticEnumOption = StaticEnum<TEnum>();
		auto EnumToString = StaticEnumOption->GetNameStringByValue(InEnumValue);

		AddDynamicOption(EnumToString, InDisplayText);
	}

	template<typename TEnum>
	TEnum GetCurrentValueAsEnum() const
	{
		const UEnum* StaticEnumOption = StaticEnum<TEnum>();
		return (TEnum)StaticEnumOption->GetValueByNameString(CurrentStringValue);
	}

	template<typename TEnum>
	void SetDefaultValueFromEnumOption(const TEnum& InEnumValue)
	{
		const UEnum* StaticEnumOption = StaticEnum<TEnum>();
		FString EnumString = StaticEnumOption->GetNameStringByValue(InEnumValue);
		SetDefaultValueFromString(EnumString);
	}
};

UCLASS()
class FRONTENDUI_API UListDataObject_StringInt : public UMyListDataObject_String
{
	GENERATED_BODY()

public:
	void AddIntegerOption(int32 Value, const FText& InDisplayText);

protected:
	virtual void OnDataObjectInitialized() override;
	virtual void OnEditDependencyDataModified(UListDataObject_Base* InDependencyDataModified, EOptionsListDataModifyReason ModifyReason) override;
};