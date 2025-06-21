// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObject_Base::AddEditCondition(const FOptionsDataEditConditionDescriptor& InEditCondition)
{
	EditContionDescArray.Add(InEditCondition);
}

void UListDataObject_Base::AddEditDependencyData(UListDataObject_Base* InDependencyData)
{
	if (!InDependencyData->OnListDataModified.IsBoundToObject(this))
	{
		InDependencyData->OnListDataModified.AddUObject(this, &ThisClass::OnEditDependencyDataModified);
	}
}

bool UListDataObject_Base::IsDataCurrentlyEditable()
{
	bool bIsEditable = true;

	if (EditContionDescArray.IsEmpty())
	{
		return bIsEditable;
	}

	FString CachedDisabledRichReason;

	for (const FOptionsDataEditConditionDescriptor& EditCondition : EditContionDescArray)
	{
		if (!EditCondition.IsValid() || EditCondition.IsConditionMet())
		{
			continue;
		}
		bIsEditable = false;

		CachedDisabledRichReason.Append(EditCondition.GetDisabledRichReason());
		SetDisabledRichText(FText::FromString(CachedDisabledRichReason));

		if (EditCondition.HasForcedStringValue())
		{
			const FString ForcedStringValue = EditCondition.GetForcedStringValue();

			if (CanSetToForcedStringValue(ForcedStringValue))
			{
				OnSetToForcedStringValue(ForcedStringValue);
			}
		}
	}
	return bIsEditable;
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

void UListDataObject_Base::OnEditDependencyDataModified(UListDataObject_Base* InDependencyDataModified, EOptionsListDataModifyReason ModifyReason)
{
	OnDependencyDataModified.Broadcast(InDependencyDataModified, ModifyReason);
}


