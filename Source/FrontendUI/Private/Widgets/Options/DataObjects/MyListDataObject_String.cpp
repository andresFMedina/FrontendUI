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

void UMyListDataObject_String::OnDataObjectInitialized()
{
	if (!AvailableOptions.IsEmpty())
	{
		CurrentStringValue = AvailableOptions[0];
	}

	//TODO: Read from the saved value
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
