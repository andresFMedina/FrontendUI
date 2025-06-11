// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataAsset_ListEntryMapping.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

TSubclassOf<UWidget_ListEntry_Base> UDataAsset_ListEntryMapping::FindEntryWidgetClassByDataObject(UListDataObject_Base* InDataObject) const
{
	check(InDataObject);

	for (UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass; DataObjectClass = DataObjectClass->GetSuperClass())
	{
		if (auto ConvertedDataObjectClass = TSubclassOf<UListDataObject_Base>(DataObjectClass))
		{
			if (ListEntryMap.Contains(ConvertedDataObjectClass))
			{
				return ListEntryMap.FindRef(ConvertedDataObjectClass);
			}
		}


	}

	return TSubclassOf<UWidget_ListEntry_Base>();
}
