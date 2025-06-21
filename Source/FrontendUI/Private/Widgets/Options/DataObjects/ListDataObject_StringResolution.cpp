// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

#include "FrontendDebugHelper.h"

void UListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> SupportedResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(SupportedResolutions);

	SupportedResolutions.Sort(
		[](const FIntPoint& A, const FIntPoint& B)
		{
			return A.SizeSquared() < B.SizeSquared();
		}
	);

	for (const FIntPoint& Resolution : SupportedResolutions)
	{
		AddDynamicOption(
			ResToValueString(Resolution),
			ResToDisplayText(Resolution)
		);
	}

	MaximumAllowedResolution = ResToValueString(SupportedResolutions.Last());

	SetDefaultValueFromString(MaximumAllowedResolution);
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();
	
	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayName = ResToDisplayText(UFrontendGameUserSettings::Get()->GetScreenResolution());
	}
}

bool UListDataObject_StringResolution::CanSetToForcedStringValue(const FString& InForcedValue) const
{
	return CurrentStringValue != InForcedValue;
}

void UListDataObject_StringResolution::OnSetToForcedStringValue(const FString& InForcedValue)
{
	CurrentStringValue = InForcedValue;
	TrySetDisplayTextFromStringValue(InForcedValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	}
}

FString UListDataObject_StringResolution::ResToValueString(const FIntPoint& Resolution) const
{
	return FString::Printf(TEXT("(X=%i,Y=%i)"), Resolution.X, Resolution.Y);	
}

FText UListDataObject_StringResolution::ResToDisplayText(const FIntPoint& Resolution) const
{
	const FString DisplayString = FString::Printf(TEXT("%i x %i"), Resolution.X, Resolution.Y);
	return FText::FromString(DisplayString);
}
