// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"

void UWidget_ListEntry_Base::NativeOnListEntryWidgetHovered(bool bWasHovered)
{
	BP_OnListEntryWidgetHovered(bWasHovered, IsListItemSelected());
}

void UWidget_ListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	SetVisibility(ESlateVisibility::Visible);

	OnOwningListDataViewSet(CastChecked<UListDataObject_Base>(ListItemObject));
}

void UWidget_ListEntry_Base::OnOwningListDataViewSet(UListDataObject_Base* InOwningDataObject)
{
	if (SettingDisplayName)
	{
		SettingDisplayName->SetText(InOwningDataObject->GetDataDisplayName());
	}

	if (!InOwningDataObject->OnListDataModified.IsBoundToObject(this))
	{
		InOwningDataObject->OnListDataModified.AddUObject(this, &ThisClass::OnOwningListDataObjectModified);
	}
	
}

void UWidget_ListEntry_Base::OnOwningListDataObjectModified(UListDataObject_Base* InOwningDataObject, EOptionsListDataModifyReason ModifyReason)
{
}

void UWidget_ListEntry_Base::SelectThisEntryWidget()
{
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}
