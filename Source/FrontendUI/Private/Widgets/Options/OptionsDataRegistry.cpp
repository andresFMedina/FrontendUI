// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/MyListDataObject_String.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "FrontendGameplayTags.h"
#include "FrontendFunctionLibrary.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "Internationalization/StringTableRegistry.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "FrontendDebugHelper.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
   MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))	

#define GET_DESCRIPTION(InKey) LOCTABLE("/Game/UI/StringTables/ST_OptionsScreenDescription.ST_OptionsScreenDescription", InKey)


void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlsCollectionTab(InOwningLocalPlayer);
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
			OverallVolumeData->SetDataDisplayName(FText::FromString("Overall Volume")); ;
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

		//Music Volume
		{
			auto MusicVolumeData = NewObject<UListDataObject_Scalar>();
			MusicVolumeData->SetDataID(FName("MusicVolume"));
			MusicVolumeData->SetDataDisplayName(FText::FromString("Music Volume")); ;
			MusicVolumeData->SetDisplayNumericType(ECommonNumericType::Number);
			MusicVolumeData->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the volume of the music in the game.")));
			MusicVolumeData->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MusicVolumeData->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MusicVolumeData->SetSliderStepSize(0.01f);
			MusicVolumeData->SetDefaultValueFromString(LexToString(1.f));
			MusicVolumeData->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolumeData->SetFormattingOptions(UListDataObject_Scalar::NoDecimal());

			MusicVolumeData->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolumeData->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
			MusicVolumeData->SetShouldApplyChangesImmediatly(true);

			VolumeCategoryCollection->AddChildListData(MusicVolumeData);
		}

		//Sound FX Volume
		{
			auto SoundFXVolumeData = NewObject<UListDataObject_Scalar>();
			SoundFXVolumeData->SetDataID(FName("SoundFX"));
			SoundFXVolumeData->SetDataDisplayName(FText::FromString("SoundFX Volume")); ;
			SoundFXVolumeData->SetDisplayNumericType(ECommonNumericType::Number);
			SoundFXVolumeData->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the sound effects of the music in the game.")));
			SoundFXVolumeData->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			SoundFXVolumeData->SetOutputValueRange(TRange<float>(0.f, 2.f));
			SoundFXVolumeData->SetSliderStepSize(0.01f);
			SoundFXVolumeData->SetDefaultValueFromString(LexToString(1.f));
			SoundFXVolumeData->SetDisplayNumericType(ECommonNumericType::Percentage);
			SoundFXVolumeData->SetFormattingOptions(UListDataObject_Scalar::NoDecimal());

			SoundFXVolumeData->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSoundFXVolume));
			SoundFXVolumeData->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSoundFXVolume));
			SoundFXVolumeData->SetShouldApplyChangesImmediatly(true);

			VolumeCategoryCollection->AddChildListData(SoundFXVolumeData);
		}
	}

	//Sound Category
	{
		auto SoundCategoryCollection = NewObject<UListDataObject_Collection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundCategoryCollection->SetDataDisplayName(FText::FromString("Sound"));
		AudioTabCollection->AddChildListData(SoundCategoryCollection);
		//Enable Background Audio
		{
			auto EnableBackgroundAudio = NewObject<UListDataObject_StringBool>();
			EnableBackgroundAudio->SetDataID(FName("EnableBackgroundAudio"));
			EnableBackgroundAudio->SetDataDisplayName(FText::FromString("Enable Background Audio"));
			EnableBackgroundAudio->SetDescriptionRichText(FText::FromString(TEXT("Enables or disables the background audio.")));
			EnableBackgroundAudio->SetFalseAsDefaultValue();

			EnableBackgroundAudio->OverrideTrueDisplayText(FText::FromString("Enabled"));
			EnableBackgroundAudio->OverrideFalseDisplayText(FText::FromString("Disabled"));
			EnableBackgroundAudio->SetTrueAsDefaultValue();
			EnableBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsBackgroundAudioEnabled));
			EnableBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetEnableBackgroundAudio));
			EnableBackgroundAudio->SetShouldApplyChangesImmediatly(true);
			SoundCategoryCollection->AddChildListData(EnableBackgroundAudio);
		}

		// Use HDR Audio
		{
			auto UseHDRAudio = NewObject<UListDataObject_StringBool>();
			UseHDRAudio->SetDataID(FName("UseHDRAudio"));
			UseHDRAudio->SetDataDisplayName(FText::FromString("Use HDR Audio Mode"));
			UseHDRAudio->SetDescriptionRichText(FText::FromString(TEXT("Enables or disables the use of HDR audio.")));
			UseHDRAudio->SetFalseAsDefaultValue();
			UseHDRAudio->OverrideTrueDisplayText(FText::FromString("Enabled"));
			UseHDRAudio->OverrideFalseDisplayText(FText::FromString("Disabled"));
			UseHDRAudio->SetTrueAsDefaultValue();
			UseHDRAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsHDRAudioEnabled));
			UseHDRAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudio));
			UseHDRAudio->SetShouldApplyChangesImmediatly(true);
			SoundCategoryCollection->AddChildListData(UseHDRAudio);
		}
	}

	RegisteredOptionsTabsCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	auto VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString("Video"));

	UListDataObject_StringEnum* CreatedWindowMode = nullptr;

	//Display Resolution Category
	{
		auto DisplayResollutionCategoryCollection = NewObject<UListDataObject_Collection>();
		DisplayResollutionCategoryCollection->SetDataID(FName("DisplayResolutionCategoryCollection"));
		DisplayResollutionCategoryCollection->SetDataDisplayName(FText::FromString("Display Resolution"));

		VideoTabCollection->AddChildListData(DisplayResollutionCategoryCollection);

		FOptionsDataEditConditionDescriptor PackageBuildOnlyCondition;
		PackageBuildOnlyCondition.SetEditConditionFunc(
			[]()->bool {
				const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;
				return !bIsInEditor;
			}
		);

		PackageBuildOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This setting can only be adjusted in package build</>"));

		// Window Mode
		{
			auto WindowModeData = NewObject<UListDataObject_StringEnum>();
			WindowModeData->SetDataID(FName("WindowMode"));
			WindowModeData->SetDataDisplayName(FText::FromString("Window Mode"));
			WindowModeData->SetDescriptionRichText(GET_DESCRIPTION("WindowModeDescKey"));
			WindowModeData->AddEnumOption(EWindowMode::Fullscreen, FText::FromString("Fullscreen"));
			WindowModeData->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString("Borderless Window"));
			WindowModeData->AddEnumOption(EWindowMode::Windowed, FText::FromString("Windowed"));
			WindowModeData->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);
			WindowModeData->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowModeData->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowModeData->SetShouldApplyChangesImmediatly(true);

			WindowModeData->AddEditCondition(PackageBuildOnlyCondition);
			CreatedWindowMode = WindowModeData;

			DisplayResollutionCategoryCollection->AddChildListData(WindowModeData);
		}

		// Screen Resolution
		{
			auto ScreenResolutionData = NewObject<UListDataObject_StringResolution>();
			ScreenResolutionData->SetDataID(FName("ScreenResolution"));
			ScreenResolutionData->SetDataDisplayName(FText::FromString("Screen Resolution"));
			ScreenResolutionData->SetDescriptionRichText(GET_DESCRIPTION("ScreenResolutionsDescKey"));

			ScreenResolutionData->InitResolutionValues();

			ScreenResolutionData->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolutionData->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolutionData->SetShouldApplyChangesImmediatly(true);
			ScreenResolutionData->AddEditCondition(PackageBuildOnlyCondition);

			FOptionsDataEditConditionDescriptor WindowModeEditCondition;

			WindowModeEditCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool {
					const bool bIsBorderlessWindow = CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen;
					return !bIsBorderlessWindow;
				}
			);

			WindowModeEditCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>Screen Resolution is not adjustable in Borderless Window</>"));
			WindowModeEditCondition.SetForcedStringValue(ScreenResolutionData->GetMaximumAllowedResolution());
			ScreenResolutionData->AddEditCondition(WindowModeEditCondition);

			ScreenResolutionData->AddEditDependencyData(CreatedWindowMode);

			DisplayResollutionCategoryCollection->AddChildListData(ScreenResolutionData);
		}
	}

	//Graphics Category
	{
		auto GraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
		GraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Graphics")));

		VideoTabCollection->AddChildListData(GraphicsCategoryCollection);

		//Display Gamma
		{
			auto DisplayGamma = NewObject<UListDataObject_Scalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDataDisplayName(FText::FromString(TEXT("Brightness")));
			DisplayGamma->SetDescriptionRichText(GET_DESCRIPTION("DisplayGammaDescKey"));
			DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f));
			DisplayGamma->SetSliderStepSize(0.01f);
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
			DisplayGamma->SetFormattingOptions(UListDataObject_Scalar::NoDecimal());
			DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
			DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));

			DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));

			GraphicsCategoryCollection->AddChildListData(DisplayGamma);
		}

		UListDataObject_StringInt* CreatedOverallQuality = nullptr;

		//Overall Quality
		{
			auto OverallQuality = NewObject<UListDataObject_StringInt>();
			OverallQuality->SetDataID(FName("OverallQuality"));
			OverallQuality->SetDataDisplayName(FText::FromString(TEXT("Overall Quality")));
			OverallQuality->SetDescriptionRichText(GET_DESCRIPTION("OverallQualityDescKey"));
			OverallQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			OverallQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			OverallQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			OverallQuality->AddIntegerOption(3, FText::FromString(TEXT("Very High")));
			OverallQuality->AddIntegerOption(4, FText::FromString(TEXT("Ultra High")));

			OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			OverallQuality->SetShouldApplyChangesImmediatly(true);

			GraphicsCategoryCollection->AddChildListData(OverallQuality);

			CreatedOverallQuality = OverallQuality;
		}

		//Resolution Scale
		{
			auto ResolutionScale = NewObject<UListDataObject_Scalar>();
			ResolutionScale->SetDataID("ResolutionScale");
			ResolutionScale->SetDataDisplayName(FText::FromString(TEXT("3D Resolution")));
			ResolutionScale->SetDescriptionRichText(GET_DESCRIPTION("ResolutionScaleDescKey"));
			ResolutionScale->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetFormattingOptions(UListDataObject_Scalar::NoDecimal());
			ResolutionScale->SetSliderStepSize(0.01);
			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			ResolutionScale->SetShouldApplyChangesImmediatly(true);

			ResolutionScale->AddEditDependencyData(CreatedOverallQuality);

			GraphicsCategoryCollection->AddChildListData(ResolutionScale);

		}

		// Global Illumination Quality
		{
			auto GlobalIllumination = NewObject<UListDataObject_StringInt>();
			GlobalIllumination->SetDataID(FName("GlobalIllumination"));
			GlobalIllumination->SetDataDisplayName(FText::FromString(TEXT("Global Illumination")));
			GlobalIllumination->SetDescriptionRichText(GET_DESCRIPTION("GlobalIlluminationQualityDescKey"));
			GlobalIllumination->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			GlobalIllumination->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			GlobalIllumination->AddIntegerOption(2, FText::FromString(TEXT("High")));
			GlobalIllumination->AddIntegerOption(3, FText::FromString(TEXT("Very High")));
			GlobalIllumination->AddIntegerOption(4, FText::FromString(TEXT("Ultra High")));

			GlobalIllumination->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIllumination->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIllumination->SetShouldApplyChangesImmediatly(true);

			GlobalIllumination->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(GlobalIllumination);

			GraphicsCategoryCollection->AddChildListData(GlobalIllumination);
		}

		//Shadow Quality
		{

			auto ShadowQuality = NewObject<UListDataObject_StringInt>();
			ShadowQuality->SetDataID(FName("ShadowQuality"));
			ShadowQuality->SetDataDisplayName(FText::FromString(TEXT("Shadow Quality")));
			ShadowQuality->SetDescriptionRichText(GET_DESCRIPTION("ShadowQualityDescKey"));
			ShadowQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ShadowQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ShadowQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ShadowQuality->AddIntegerOption(3, FText::FromString(TEXT("Very High")));
			ShadowQuality->AddIntegerOption(4, FText::FromString(TEXT("Ultra High")));

			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQuality->SetShouldApplyChangesImmediatly(true);

			ShadowQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ShadowQuality);

			GraphicsCategoryCollection->AddChildListData(ShadowQuality);
		}

		//AntiAliasing Quality
		{
			auto AntiAliasingQuality = NewObject<UListDataObject_StringInt>();
			AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
			AntiAliasingQuality->SetDataDisplayName(FText::FromString(TEXT("Anti Aliasing")));
			AntiAliasingQuality->SetDescriptionRichText(GET_DESCRIPTION("AntiAliasingDescKey"));
			AntiAliasingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			AntiAliasingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			AntiAliasingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			AntiAliasingQuality->AddIntegerOption(3, FText::FromString(TEXT("Very High")));
			AntiAliasingQuality->AddIntegerOption(4, FText::FromString(TEXT("Ultra High")));

			AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
			AntiAliasingQuality->SetShouldApplyChangesImmediatly(true);

			AntiAliasingQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(AntiAliasingQuality);

			GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
		}

		//View Distance Quality
		{
			auto ViewDistanceQuality = NewObject<UListDataObject_StringInt>();
			ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
			ViewDistanceQuality->SetDataDisplayName(FText::FromString(TEXT("View Distance Quality")));
			ViewDistanceQuality->SetDescriptionRichText(GET_DESCRIPTION("ViewDistanceDescKey"));
			ViewDistanceQuality->AddIntegerOption(0, FText::FromString(TEXT("Near")));
			ViewDistanceQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ViewDistanceQuality->AddIntegerOption(2, FText::FromString(TEXT("Far")));
			ViewDistanceQuality->AddIntegerOption(3, FText::FromString(TEXT("Very Far")));
			ViewDistanceQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistanceQuality->SetShouldApplyChangesImmediatly(true);

			ViewDistanceQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ViewDistanceQuality);

			GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
		}

		//Texture Quality
		{
			auto TextureQuality = NewObject<UListDataObject_StringInt>();
			TextureQuality->SetDataID(FName("TextureQuality"));
			TextureQuality->SetDataDisplayName(FText::FromString(TEXT("Texture Quality")));
			TextureQuality->SetDescriptionRichText(GET_DESCRIPTION("TextureQualityDescKey"));
			TextureQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			TextureQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			TextureQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			TextureQuality->AddIntegerOption(3, FText::FromString(TEXT("Very High")));
			TextureQuality->AddIntegerOption(4, FText::FromString(TEXT("Ultra High")));

			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQuality->SetShouldApplyChangesImmediatly(true);

			TextureQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(TextureQuality);

			GraphicsCategoryCollection->AddChildListData(TextureQuality);
		}

		//Visual Effects Quality
		{
			{
				auto VisualEffectQuality = NewObject<UListDataObject_StringInt>();
				VisualEffectQuality->SetDataID(FName("VisualEffectQuality"));
				VisualEffectQuality->SetDataDisplayName(FText::FromString(TEXT("Visual Effect Quality")));
				VisualEffectQuality->SetDescriptionRichText(GET_DESCRIPTION("VisualEffectQualityDescKey"));
				VisualEffectQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
				VisualEffectQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
				VisualEffectQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
				VisualEffectQuality->AddIntegerOption(3, FText::FromString(TEXT("Very High")));
				VisualEffectQuality->AddIntegerOption(4, FText::FromString(TEXT("Ultra High")));

				VisualEffectQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
				VisualEffectQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
				VisualEffectQuality->SetShouldApplyChangesImmediatly(true);

				VisualEffectQuality->AddEditDependencyData(CreatedOverallQuality);

				CreatedOverallQuality->AddEditDependencyData(VisualEffectQuality);

				GraphicsCategoryCollection->AddChildListData(VisualEffectQuality);
			}
		}

		//Reflection Quality
		{

			auto ReflectionQuality = NewObject<UListDataObject_StringInt>();
			ReflectionQuality->SetDataID(FName("ReflectionQuality"));
			ReflectionQuality->SetDataDisplayName(FText::FromString(TEXT("Reflection Quality")));
			ReflectionQuality->SetDescriptionRichText(GET_DESCRIPTION("ReflectionQualityDescKey"));
			ReflectionQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ReflectionQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ReflectionQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ReflectionQuality->AddIntegerOption(3, FText::FromString(TEXT("Very High")));
			ReflectionQuality->AddIntegerOption(4, FText::FromString(TEXT("Ultra High")));

			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQuality->SetShouldApplyChangesImmediatly(true);

			ReflectionQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ReflectionQuality);

			GraphicsCategoryCollection->AddChildListData(ReflectionQuality);

		}

		//Post Procesing Quality
		{
			auto PostProcessingQuality = NewObject<UListDataObject_StringInt>();
			PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
			PostProcessingQuality->SetDataDisplayName(FText::FromString(TEXT("Post Processing Quality")));
			PostProcessingQuality->SetDescriptionRichText(GET_DESCRIPTION("PostProcessingQualityDescKey"));
			PostProcessingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			PostProcessingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			PostProcessingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			PostProcessingQuality->AddIntegerOption(3, FText::FromString(TEXT("Very High")));
			PostProcessingQuality->AddIntegerOption(4, FText::FromString(TEXT("Ultra High")));

			PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessingQuality->SetShouldApplyChangesImmediatly(true);

			PostProcessingQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(PostProcessingQuality);

			GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
		}
	}

	//Advanced Graphics Category
	{
		auto AdvancedGraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		AdvancedGraphicsCategoryCollection->SetDataID(FName("AdvancedGraphicsCategoryCollection"));
		AdvancedGraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Advanced Graphics")));

		VideoTabCollection->AddChildListData(AdvancedGraphicsCategoryCollection);

		//Vertical Sync
		{
			auto VerticalSync = NewObject<UListDataObject_StringBool>();
			VerticalSync->SetDataID(FName("VerticalSync"));
			VerticalSync->SetDataDisplayName(FText::FromString(TEXT("V-Sync")));
			VerticalSync->SetDescriptionRichText(GET_DESCRIPTION("VerticalSyncDescKey"));
			VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VerticalSync->SetFalseAsDefaultValue();
			VerticalSync->SetShouldApplyChangesImmediatly(true);

			FOptionsDataEditConditionDescriptor FullscreenOnlyCondition;
			FullscreenOnlyCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool
				{
					return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
				}
			);
			FullscreenOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This feature only works on Fullscreen.</>"));
			FullscreenOnlyCondition.SetForcedStringValue(TEXT("false"));
			VerticalSync->AddEditCondition(FullscreenOnlyCondition);

			AdvancedGraphicsCategoryCollection->AddChildListData(VerticalSync);
		}

		//Frame Rate Limit
		{
			auto FrameRateLimit = NewObject<UMyListDataObject_String>();
			FrameRateLimit->SetDataID(FName("FrameRateLimit"));
			FrameRateLimit->SetDataDisplayName(FText::FromString(TEXT("Frame Rate Limit")));
			FrameRateLimit->SetDescriptionRichText(GET_DESCRIPTION("FrameRateLimitDescKey"));
			FrameRateLimit->AddDynamicOption(LexToString(30.f), FText::FromString(TEXT("30 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(60.f), FText::FromString(TEXT("60 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(90.f), FText::FromString(TEXT("90 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(120.f), FText::FromString(TEXT("120 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(0.f), FText::FromString(TEXT("No Limit")));
			FrameRateLimit->SetDefaultValueFromString(LexToString(0.f));
			FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
			FrameRateLimit->SetShouldApplyChangesImmediatly(true);

			AdvancedGraphicsCategoryCollection->AddChildListData(FrameRateLimit);
		}
	}

	RegisteredOptionsTabsCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlsCollectionTab(ULocalPlayer* InOwningLocalPlayer)
{
	auto ControlsTabCollection = NewObject<UListDataObject_Collection>();
	ControlsTabCollection->SetDataID(FName("ControlsTabCollection"));
	ControlsTabCollection->SetDataDisplayName(FText::FromString("Controls"));

	auto EISubsystem = InOwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	check(EISubsystem);

	UEnhancedInputUserSettings* EIUserSettings = EISubsystem->GetUserSettings();

	check(EIUserSettings);

	//Keyboard Mouse Category

	{
		auto KeyboardMouseCategoryCollection = NewObject<UListDataObject_Collection>();
		KeyboardMouseCategoryCollection->SetDataID(FName("KeyboardMouseCategoryCollection"));
		KeyboardMouseCategoryCollection->SetDataDisplayName(FText::FromString("Keyboard & Mouse"));

		ControlsTabCollection->AddChildListData(KeyboardMouseCategoryCollection);

		//Keyboard Mouse Inputs

		{
			FPlayerMappableKeyQueryOptions KeyboardMouseOnly;
			KeyboardMouseOnly.KeyToMatch = EKeys::S;
			KeyboardMouseOnly.bMatchBasicKeyTypes = true;
			
			GetKeyMappingByInput(KeyboardMouseOnly, ECommonInputType::MouseAndKeyboard, EIUserSettings, KeyboardMouseCategoryCollection);
			
		}
	}

	//Gamepad Category
	{
		auto GamepadCategoryCollection = NewObject<UListDataObject_Collection>();
		GamepadCategoryCollection->SetDataID(FName("GamepadCategoryCollection"));
		GamepadCategoryCollection->SetDataDisplayName(FText::FromString("Keyboard & Mouse"));

		ControlsTabCollection->AddChildListData(GamepadCategoryCollection);

		//Gamepad Inputs
		{			
			FPlayerMappableKeyQueryOptions GamepadOnly;
			GamepadOnly.KeyToMatch = EKeys::Gamepad_FaceButton_Bottom;
			GamepadOnly.bMatchBasicKeyTypes = true;

			GetKeyMappingByInput(GamepadOnly, ECommonInputType::Gamepad, EIUserSettings, GamepadCategoryCollection);
		}
	}

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

void UOptionsDataRegistry::GetKeyMappingByInput(const FPlayerMappableKeyQueryOptions& QueryOptions, const ECommonInputType InputType, UEnhancedInputUserSettings* EIUserSettings, UListDataObject_Collection* ParentObject)
{
	for (const auto ProfilePair : EIUserSettings->GetAllSavedKeyProfiles())
	{
		auto MappableKeyProfile = ProfilePair.Value;
		check(MappableKeyProfile);

		for (auto MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
		{
			for (auto KeyMapping : MappingRowPair.Value.Mappings)
			{
				if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, QueryOptions))
				{
					auto KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
					KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
					KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());
					KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, InputType, KeyMapping);					

					ParentObject->AddChildListData(KeyRemapDataObject);
				}
			}
		}
	}
}
