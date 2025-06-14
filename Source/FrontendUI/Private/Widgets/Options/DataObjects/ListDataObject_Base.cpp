// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObject_Base::OnDataObjectInitialized()
{
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* InDataModified, EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(InDataModified, ModifyReason);
	if (bShouldApplyChangesImmediatly)
	{
		UFrontendGameUserSettings::Get()->ApplySettings(true);
	}
}


