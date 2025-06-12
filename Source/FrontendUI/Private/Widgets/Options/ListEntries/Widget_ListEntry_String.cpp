// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/Widget_ListEntry_String.h"
#include "Widgets/Options/DataObjects/MyListDataObject_String.h"
#include "Widgets/Components/FrontendCommonRotator.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"

#include "FrontendDebugHelper.h"

void UWidget_ListEntry_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_Previous->OnClicked().AddUObject(this, &UWidget_ListEntry_String::OnPreviousOptionButtonClicked);
	CommonButton_Next->OnClicked().AddUObject(this, &UWidget_ListEntry_String::OnNextOptionButtonClicked);

	CommonRotator_AvailableOptions->OnClicked().AddLambda([this]()
		{
			SelectThisEntryWidget();
		});
}

void UWidget_ListEntry_String::OnOwningListDataViewSet(UListDataObject_Base* InOwningDataObject)
{
	Super::OnOwningListDataViewSet(InOwningDataObject);

	OwningStringDataObject = CastChecked<UMyListDataObject_String>(InOwningDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(OwningStringDataObject->GetAvailableOptionsDisplayNames());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(OwningStringDataObject->GetCurrentDisplayName());
}

void UWidget_ListEntry_String::OnOwningListDataObjectModified(UListDataObject_Base* InOwningDataObject, EOptionsListDataModifyReason ModifyReason)
{
	if (OwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectedOptionByText(OwningStringDataObject->GetCurrentDisplayName());
	}
}

void UWidget_ListEntry_String::OnPreviousOptionButtonClicked()
{
	if (OwningStringDataObject)
	{
		OwningStringDataObject->BackToPreviousOption();
	}

	SelectThisEntryWidget();
}

void UWidget_ListEntry_String::OnNextOptionButtonClicked()
{
	if (OwningStringDataObject)
	{
		OwningStringDataObject->AdvanceToNextOption();
	}
	SelectThisEntryWidget();
}
