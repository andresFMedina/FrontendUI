// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncActions/AsyncAction_PushSoftWidget.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Widget_ActivatableBase.h"

UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject, APlayerController* OwningPlayerController, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag InWidgetTag, bool bFocusOnNewlyPushedWidget)
{
	checkf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToStack was passed a null soft widget class"));

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			auto Node = NewObject<UAsyncAction_PushSoftWidget>();

			Node->CachedOwningWorld = World;
			Node->CachedOwningPlayerController = OwningPlayerController;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->CachedWidgetTag = InWidgetTag;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;

			Node->RegisterWithGameInstance(World);



			return Node;
		}
	}

	return nullptr;
}

void UAsyncAction_PushSoftWidget::Activate()
{
	//Super::Activate();

	auto FrontendUISubsystem = UFrontendUISubsystem::Get(CachedOwningWorld.Get());

	FrontendUISubsystem->PushSoftWidgetToStackAsync(
		CachedWidgetTag,
		CachedSoftWidgetClass,
		[this](EAsyncPushWidgetState PushState, UWidget_ActivatableBase* PushedWidget)
		{
			switch (PushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				PushedWidget->SetOwningPlayer(CachedOwningPlayerController.Get());

				OnWidgetCreatedBeforePushed.Broadcast(PushedWidget);
				break;
			case EAsyncPushWidgetState::AfterPush:

				AfterPushed.Broadcast(PushedWidget);

				if (bCachedFocusOnNewlyPushedWidget)
				{
					if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
					{
						WidgetToFocus->SetFocus();
					}
				}

				SetReadyToDestroy();

				break;
			default:
				break;
			}
		}
	);
}
