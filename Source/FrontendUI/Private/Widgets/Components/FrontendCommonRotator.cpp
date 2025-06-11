// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/FrontendCommonRotator.h"
#include "CommonTextBlock.h"

void UFrontendCommonRotator::SetSelectedOptionByText(const FText& InText)
{
	const int32 FoundIndex = TextLabels.IndexOfByPredicate(
		[&InText](const FText& Text) { return Text.EqualTo(InText); }
	);

	if (FoundIndex != INDEX_NONE)
	{
		SetSelectedItem(FoundIndex);
	}
	else
	{
		MyText->SetText(InText);
		UE_LOG(LogTemp, Warning, TEXT("Text not found in rotator options: %s"), *InText.ToString());
	}

}
