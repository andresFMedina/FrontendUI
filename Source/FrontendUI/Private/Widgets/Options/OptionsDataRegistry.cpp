// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/MyListDataObject_String.h"


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

	return FoundTabCollection->GetAllChildListData();
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
		GameDifficultyData->AddDynamicOption(TEXT("Easy"), FText::FromString("Easy"));
		GameDifficultyData->AddDynamicOption(TEXT("Normal"), FText::FromString("Normal"));
		GameDifficultyData->AddDynamicOption(TEXT("Hard"), FText::FromString("Hard"));


		GameplayTabCollection->AddChildListData(GameDifficultyData);
	}

	//Test Item
	{
		auto TestItem = NewObject<UMyListDataObject_String>();
		TestItem->SetDataID(FName("TestItem"));
		TestItem->SetDataDisplayName(FText::FromString("Test Item"));
		GameplayTabCollection->AddChildListData(TestItem);
	}

	RegisteredOptionsTabsCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	auto AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString("Audio"));

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
