// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/MyListDataObject_String.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendDebugHelper.h"


void UMyListDataObject_String::AddDynamicOption(const FString& InOption, const FText& InDisplayName)
{
	AvailableOptions.Add(InOption);
	AvailableOptionsDisplayNames.Add(InDisplayName);
	
}

void UMyListDataObject_String::AdvanceToNextOption()
{
	if (AvailableOptions.IsEmpty() || AvailableOptionsDisplayNames.IsEmpty())
	{
		return;
	}

	const int32 CurrentFoundIndex = AvailableOptions.IndexOfByKey(CurrentStringValue);
	const int32 NextIndexToDisplay = CurrentFoundIndex + 1;

	if (AvailableOptions.IsValidIndex(NextIndexToDisplay))
	{
		CurrentStringValue = AvailableOptions[NextIndexToDisplay];
	}
	else
	{		
		CurrentStringValue = AvailableOptions[0];
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		Debug::Print(TEXT("DataDynamic setter used. Latest Value = ") + DataDynamicGetter->GetValueAsString());

		NotifyListDataModified(this);
	}
	
}

void UMyListDataObject_String::BackToPreviousOption()
{
	if (AvailableOptions.IsEmpty() || AvailableOptionsDisplayNames.IsEmpty())
	{
		return;
	}

	const int32 CurrentFoundIndex = AvailableOptions.IndexOfByKey(CurrentStringValue);
	const int32 PreviousIndexToDisplay = CurrentFoundIndex - 1;

	if (AvailableOptions.IsValidIndex(PreviousIndexToDisplay))
	{
		CurrentStringValue = AvailableOptions[PreviousIndexToDisplay];
	}
	else
	{
		CurrentStringValue = AvailableOptions.Last();
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		Debug::Print(TEXT("DataDynamic setter used. Latest Value = ") + DataDynamicGetter->GetValueAsString());

		NotifyListDataModified(this);
	}
}

void UMyListDataObject_String::OnRotatorInitiatedValueChange(const FText& InNewSelectedText)
{
	int32 FoundIndex = AvailableOptionsDisplayNames.IndexOfByPredicate([InNewSelectedText](const FText& Option)
		{
			return Option.EqualTo(InNewSelectedText);
		});

	if (FoundIndex != INDEX_NONE && AvailableOptions.IsValidIndex(FoundIndex)) 
	{
		CurrentDisplayName = InNewSelectedText;
		CurrentStringValue = AvailableOptions[FoundIndex];

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);

			NotifyListDataModified(this);
		}
	}
		
}

bool UMyListDataObject_String::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && CurrentStringValue != GetDefaultValueAsString();
}

bool UMyListDataObject_String::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();

		TrySetDisplayTextFromStringValue(CurrentStringValue);

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);			
			NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);
			return true;
		}		
	}
	return false;
}

void UMyListDataObject_String::OnDataObjectInitialized()
{
	if (!AvailableOptions.IsEmpty())
	{
		CurrentStringValue = AvailableOptions[0];
	}

	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
	}
	
	if (DataDynamicGetter)
	{
		if (!DataDynamicGetter->GetValueAsString().IsEmpty()) 
		{
			CurrentStringValue = DataDynamicGetter->GetValueAsString();
		}
	}

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayName = FText::FromString(TEXT("Invalid Option"));
	}

}

bool UMyListDataObject_String::TrySetDisplayTextFromStringValue(const FString& InStringValue)
{
	const int32 CurrentFoundIndex = AvailableOptions.IndexOfByKey(InStringValue);
	if (AvailableOptions.IsValidIndex(CurrentFoundIndex))
	{		
		CurrentDisplayName = AvailableOptionsDisplayNames[CurrentFoundIndex];
		return true;
	}

	return false;
}

/**************************************** UListDataObject_StringBool          ************************************/

void UListDataObject_StringBool::OverrideTrueDisplayText(const FText& InTrueDisplayText)
{
	if (!AvailableOptions.Contains(TrueString))
	{
		AddDynamicOption(TrueString, InTrueDisplayText);
	}
}

void UListDataObject_StringBool::OverrideFalseDisplayText(const FText& InFalseDisplayText)
{
	if (!AvailableOptions.Contains(FalseString))
	{
		AddDynamicOption(FalseString, InFalseDisplayText);
	}
}

void UListDataObject_StringBool::SetTrueAsDefaultValue()
{
	SetDefaultValueFromString(TrueString);
}

void UListDataObject_StringBool::SetFalseAsDefaultValue()
{
	SetDefaultValueFromString(FalseString);
}

void UListDataObject_StringBool::OnDataObjectInitialized()
{
	TryInitBoolValues();

	Super::OnDataObjectInitialized();
}

void UListDataObject_StringBool::TryInitBoolValues()
{
	if (!AvailableOptions.Contains(TrueString))
	{
		AddDynamicOption(TrueString, FText::FromString(TEXT("ON")));
	}
	if (!AvailableOptions.Contains(FalseString))
	{
		AddDynamicOption(FalseString, FText::FromString(TEXT("OFF")));
	}
}

/**************************************** UListDataObject_StringBool          ************************************/

/****************************************UListDataObject_StringInt************************************ */

void UListDataObject_StringInt::AddIntegerOption(int32 Value, const FText& InDisplayText)
{
	AddDynamicOption(LexToString(Value), InDisplayText);
}

void UListDataObject_StringInt::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayName = FText::FromString(TEXT("Custom"));
	}
}

void UListDataObject_StringInt::OnEditDependencyDataModified(UListDataObject_Base* InDependencyDataModified, EOptionsListDataModifyReason ModifyReason)
{
	if (DataDynamicGetter)
	{
		if (CurrentStringValue == DataDynamicGetter->GetValueAsString()) 
		{
			return;
		}
		CurrentStringValue = DataDynamicGetter->GetValueAsString();

		if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
		{
			CurrentDisplayName = FText::FromString(TEXT("Custom"));
		}

		NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	}

	Super::OnEditDependencyDataModified(InDependencyDataModified, ModifyReason);
}
