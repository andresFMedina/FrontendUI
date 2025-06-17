// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/MyListDataObject_String.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "FrontendGameplayTags.h"
#include "FrontendFunctionLibrary.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
   MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))	

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlsCollectionTab();
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabId(const FName& TabID) const
{
	auto FoundTab = RegisteredOptionsTabsCollections.FindByPredicate(
		[&TabID](const UListDataObject_Collection* Collection) { return Collection && Collection->GetDataID() == TabID; }
	);

	checkf(FoundTab, TEXT("No collection found for TabID: %s"), *TabID.ToString());
	
	auto FoundTabCollection = *FoundTab;

	TArray<UListDataObject_Base*> AllChildListItems;

	for (const auto& ChildData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildData)
		{
			continue;
		}

		AllChildListItems.Add(ChildData);

		if (ChildData->HasAnyChildListData())
		{
			FindChildListDataRecurively(ChildData, AllChildListItems);
		}
	}

	return AllChildListItems;
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	auto GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString("Gameplay"));

	//Game Difficulty
	{
		auto GameDifficultyData = NewObject<UMyListDataObject_String>();
		GameDifficultyData->SetDataID(FName("GameDifficulty"));
		GameDifficultyData->SetDataDisplayName(FText::FromString("Game Difficulty"));
		GameDifficultyData->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the difficulty of the game experience.\n\n<Bold>Easy:</> Focuses on the story experience. Provides the most relaxing combat.\n\n<Bold>Normal:</> Offers slightly harder combat experience\n\n<Bold>Hard:</> Offers a much more challenging combat experience\n\n<Bold>Vert Hard:</> Provides the most challenging combat experience. Not recommended for first play through.")));
		GameDifficultyData->AddDynamicOption(TEXT("Easy"), FText::FromString("Easy"));
		GameDifficultyData->AddDynamicOption(TEXT("Normal"), FText::FromString("Normal"));
		GameDifficultyData->AddDynamicOption(TEXT("Hard"), FText::FromString("Hard"));

		GameDifficultyData->SetDefaultValueFromString(TEXT("Normal"));

		GameDifficultyData->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficultyData->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));

		GameDifficultyData->SetShouldApplyChangesImmediatly(true);

		GameplayTabCollection->AddChildListData(GameDifficultyData);
	}

	//Test Item
	{
		auto TestItem = NewObject<UMyListDataObject_String>();
		TestItem->SetDataID(FName("TestItem"));
		TestItem->SetDataDisplayName(FText::FromString("Test Image Item"));
		TestItem->SetSoftDescriptionImage(UFrontendFunctionLibrary::GetOptionSoftImageByTag(FrontendGameplayTags::Frontend_Image_TestImage));
		GameplayTabCollection->AddChildListData(TestItem);
	}

	RegisteredOptionsTabsCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	auto AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString("Audio"));

	//Volume
	{
		auto VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString("Volume"));

		AudioTabCollection->AddChildListData(VolumeCategoryCollection);

		//Overall Volume
		{
			auto OverallVolumeData = NewObject<UListDataObject_Scalar>();
			OverallVolumeData->SetDataID(FName("OverallVolume"));
			OverallVolumeData->SetDataDisplayName(FText::FromString("Overall Volume"));						;
			OverallVolumeData->SetDisplayNumericType(ECommonNumericType::Number);
			OverallVolumeData->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the overall volume of the game.")));
			OverallVolumeData->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			OverallVolumeData->SetOutputValueRange(TRange<float>(0.f, 2.f));
			OverallVolumeData->SetSliderStepSize(0.01f);
			OverallVolumeData->SetDefaultValueFromString(LexToString(1.f));
			OverallVolumeData->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolumeData->SetFormattingOptions(UListDataObject_Scalar::NoDecimal());
			
			OverallVolumeData->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolumeData->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
			OverallVolumeData->SetShouldApplyChangesImmediatly(true);
			
			VolumeCategoryCollection->AddChildListData(OverallVolumeData);
		}
	}

	RegisteredOptionsTabsCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	auto VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString("Video"));

	RegisteredOptionsTabsCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlsCollectionTab()
{
	auto ControlsTabCollection = NewObject<UListDataObject_Collection>();
	ControlsTabCollection->SetDataID(FName("ControlsTabCollection"));
	ControlsTabCollection->SetDataDisplayName(FText::FromString("Controls"));

	RegisteredOptionsTabsCollections.Add(ControlsTabCollection);
}

void UOptionsDataRegistry::FindChildListDataRecurively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutChildData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}
	for (const auto& ChildData : InParentData->GetAllChildListData())
	{
		if (!ChildData)
		{
			continue;
		}
		OutChildData.Add(ChildData);
		if (ChildData->HasAnyChildListData())
		{
			FindChildListDataRecurively(ChildData, OutChildData);
		}
	}
}
