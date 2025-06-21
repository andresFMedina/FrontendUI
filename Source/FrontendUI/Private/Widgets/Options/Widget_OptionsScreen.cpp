 // Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/Widget_OptionsScreen.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "FrontendDebugHelper.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Components/FrontendCommonListView.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widgets/Options/Widget_OptionsDetailsView.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"


void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionBindingHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetAction,
				true,
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered))
		);
	}

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered))
	);

	TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(
		this,
		&ThisClass::OnOptionsTabSelected
	);

	CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(
		this,
		&ThisClass::OnListViewItemHovered
	);

	CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(
		this,
		&ThisClass::OnListViewItemSelected
	);
}

void UWidget_OptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (auto TabCollection : GetOrCreateDataRegistry() -> GetRegisteredOptionsTabsCollections()) 
	{
		if (!TabCollection)
		{
			continue;
		}
		const FName TabID = TabCollection->GetDataID();

		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID) != nullptr)
		{			
			continue;
		}

		TabListWidget_OptionsTabs->RequestRegisterTab(
			TabID,
			TabCollection->GetDataDisplayName()
		);
	}

}

void UWidget_OptionsScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UFrontendGameUserSettings::Get()->ApplySettings(true);
}

UWidget* UWidget_OptionsScreen::NativeGetDesiredFocusTarget() const
{
	if (UObject* SelectedObject = CommonListView_OptionsList->GetSelectedItem())
	{
		if (auto SelectedWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(SelectedObject))
		{
			return SelectedWidget;
		}
	}

	return Super::NativeGetDesiredFocusTarget();
}

UOptionsDataRegistry* UWidget_OptionsScreen::GetOrCreateDataRegistry()
{
	if (!OwningDataRegistry)
	{
		OwningDataRegistry = NewObject<UOptionsDataRegistry>();
		OwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}

	checkf(OwningDataRegistry, TEXT("Data Registry is not valid"));

	return OwningDataRegistry;
	
}

void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	if (ResetableData.IsEmpty())
	{
		return;
	}

	UCommonButtonBase* SelectedTab = TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabListWidget_OptionsTabs->GetActiveTab());
	const auto SelectedTabName = CastChecked<UFrontendCommonButtonBase>(SelectedTab)->GetButtonDisplayText().ToString();

	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Reset Settings")),
		FText::FromString(TEXT("Are you sure you want to reset all settings under ") + SelectedTabName + TEXT(" to default ? This action cannot be undone.")),
		[this](EConfirmScreenButtonType ButtonType)
		{
			if (ButtonType != EConfirmScreenButtonType::Confirmed)
			{
				return;				
			}

			bIsResettingData = true;
			bool bHasDataFailedToReset = false;

			for (auto ResetableItem : ResetableData)
			{
				if (!ResetableItem)
				{
					continue;
				}

				if (ResetableItem->TryResetBackToDefaultValue())
				{
					Debug::Print(FString::Printf(TEXT("Resetting %s to default value."), *ResetableItem->GetDataDisplayName().ToString()));
				}
				else
				{
					bHasDataFailedToReset = true;
					Debug::Print(FString::Printf(TEXT("Failed to reset %s to default value."), *ResetableItem->GetDataDisplayName().ToString()));
				}
			}

			if (!bHasDataFailedToReset)
			{
				ResetableData.Empty();
				RemoveActionBinding(ResetActionBindingHandle);
			}

			bIsResettingData = false;
		}
	);
}

void UWidget_OptionsScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabId)
{
	DetailsView_ListEntryInfo->ClearDetailsViewInfo();

	auto FoundListItems = GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabId(TabId);
	CommonListView_OptionsList->SetListItems(FoundListItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetListItems().Num() > 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}
	ResetableData.Empty();

	for (auto Item : FoundListItems)
	{
		if (!Item)
		{
			continue;
		}
		if (!Item->OnListDataModified.IsBoundToObject(this))
		{
			Item->OnListDataModified.AddUObject(
				this,
				&ThisClass::OnListViewDataModified
			);
		}

		if (Item->CanResetBackToDefaultValue())
		{
			ResetableData.AddUnique(Item);
		}
	}

	if (ResetableData.IsEmpty())
	{
		RemoveActionBinding(ResetActionBindingHandle);
	}
	else 
	{
		if (!GetActionBindings().Contains(ResetActionBindingHandle))
		{
			AddActionBinding(ResetActionBindingHandle);
		}
	}
}

void UWidget_OptionsScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bIsHovered)
{
	if (!InHoveredItem)
	{
		return;
	}

	auto HoveredEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);
	check(HoveredEntryWidget);

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bIsHovered);

	if (bIsHovered)
	{
		DetailsView_ListEntryInfo->UpdateDetailsViewInfo(
			CastChecked<UListDataObject_Base>(InHoveredItem),
			TryGetEntryWidgetClassName(InHoveredItem)
		);
	}
	else
	{
		if (auto SelectedItem = CommonListView_OptionsList->GetSelectedItem<UListDataObject_Base>())
		{
			DetailsView_ListEntryInfo->UpdateDetailsViewInfo(
				SelectedItem,
				TryGetEntryWidgetClassName(SelectedItem)
			);
		}		
	}
}

void UWidget_OptionsScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem)
	{
		return;
	}

	DetailsView_ListEntryInfo->UpdateDetailsViewInfo(
		CastChecked<UListDataObject_Base>(InSelectedItem),
		TryGetEntryWidgetClassName(InSelectedItem)
	);
	
}

FString UWidget_OptionsScreen::TryGetEntryWidgetClassName(UObject* InSelectedItem) const
{
	if (UUserWidget* FoundWidget = CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InSelectedItem))
	{
		return FoundWidget->GetClass()->GetName();
	}
	else
	{
		return FString(TEXT("Unknown Entry Widget Class"));
	}
}

void UWidget_OptionsScreen::OnListViewDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (!ModifiedData || bIsResettingData)
	{
		return;
	}

	if (ModifiedData->CanResetBackToDefaultValue()) 
	{
		ResetableData.AddUnique(ModifiedData);

		if (!GetActionBindings().Contains(ResetActionBindingHandle))
		{
			AddActionBinding(ResetActionBindingHandle);
		}
	}
	else
	{
		if (ResetableData.Contains(ModifiedData))
		{
			ResetableData.Remove(ModifiedData);
		}
	}

	if (ResetableData.IsEmpty())
	{
		RemoveActionBinding(ResetActionBindingHandle);
	}
}
