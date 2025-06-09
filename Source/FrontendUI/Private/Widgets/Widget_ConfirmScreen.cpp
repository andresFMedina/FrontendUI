// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_ConfirmScreen.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "ICommonInputModule.h"
#include "Components/DynamicEntryBox.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMessage)
{
	auto InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo OKButtonInfo;
	OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
	OKButtonInfo.ButtonTextToDisplay = FText::FromString("OK");

	InfoObject->AvailableScreenButtons.Add(OKButtonInfo);

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage)
{

	auto InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString("Yes");

	FConfirmScreenButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString("No");

	InfoObject->AvailableScreenButtons.Add(YesButtonInfo);
	InfoObject->AvailableScreenButtons.Add(NoButtonInfo);

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage)
{
	auto InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	OkButtonInfo.ButtonTextToDisplay = FText::FromString("Ok");

	FConfirmScreenButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	CancelButtonInfo.ButtonTextToDisplay = FText::FromString("Cancel");

	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);

	return InfoObject;
}

void UWidget_ConfirmScreen::InitConfirmScreen(const UConfirmScreenInfoObject* InConfirmScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
	check(InConfirmScreenInfoObject && CommonTextBlock_Title && CommonTextBlock_Message && DynamicEntryBox_Buttons);

	CommonTextBlock_Title->SetText(InConfirmScreenInfoObject->ScreenTitle);
	CommonTextBlock_Message->SetText(InConfirmScreenInfoObject->ScreenMessage);

	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		DynamicEntryBox_Buttons->Reset<UFrontendCommonButtonBase>(
			[](UFrontendCommonButtonBase& Button)
			{
				Button.OnClicked().Clear();
			},
			true
		);
	}

	check(!InConfirmScreenInfoObject->AvailableScreenButtons.IsEmpty());

	for (const FConfirmScreenButtonInfo& ButtonInfo : InConfirmScreenInfoObject->AvailableScreenButtons)
	{
		FDataTableRowHandle InputActionRowHandle;

		auto ButtonWidget = DynamicEntryBox_Buttons->CreateEntry<UFrontendCommonButtonBase>();
		if (ButtonWidget)
		{
			switch (ButtonInfo.ConfirmScreenButtonType)
			{
			case EConfirmScreenButtonType::Cancelled:
				InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
				break;
			case EConfirmScreenButtonType::Closed:
				InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
				break;
			default:
				break;
			}


			ButtonWidget->SetButtonText(ButtonInfo.ButtonTextToDisplay);
			ButtonWidget->SetTriggeringInputAction(InputActionRowHandle);
			ButtonWidget->OnClicked().AddLambda([this, ClickedButtonCallback, ButtonInfo]()
				{
					ClickedButtonCallback(ButtonInfo.ConfirmScreenButtonType);

					DeactivateWidget();
				});
		}
	}

	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		DynamicEntryBox_Buttons->GetAllEntries().Last()->SetFocus();
	}
}
