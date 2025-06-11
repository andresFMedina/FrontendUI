// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "CommonTextBlock.h"

void UWidget_ListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

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
