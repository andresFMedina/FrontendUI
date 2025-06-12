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
	Debug::Print(TEXT("Reset action triggered!"));
}

void UWidget_OptionsScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabId)
{
	auto FoundListItems = GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabId(TabId);
	CommonListView_OptionsList->SetListItems(FoundListItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetListItems().Num() > 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
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
}

void UWidget_OptionsScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem)
	{
		return;
	}
	Debug::Print(
		FString::Printf(TEXT("Selected item: %s"), *InSelectedItem->GetName()),
		-1,
		5.0f,
		FColor::Blue
	);
}
