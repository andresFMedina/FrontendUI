// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonTextBlock.h"

void UFrontendCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);

}

void UFrontendCommonButtonBase::SetButtonText(FText InText)
{
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUseUpperCaseButtonText? InText.ToUpper() : InText);
	}
}
