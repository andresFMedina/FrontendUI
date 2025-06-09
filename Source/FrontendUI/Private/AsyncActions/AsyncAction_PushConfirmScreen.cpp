// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncActions/AsyncAction_PushConfirmScreen.h"
#include "Subsystems/FrontendUISubsystem.h"


UAsyncAction_PushConfirmScreen* UAsyncAction_PushConfirmScreen::ShowConfirmationScreen(const UObject* WorldContextObject, EConfirmScreenType InScreenType, FText InScreenTitle, FText InScreenMessage)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		auto Node = NewObject<UAsyncAction_PushConfirmScreen>(World, UAsyncAction_PushConfirmScreen::StaticClass());
		Node->CachedOwningWorld = World;
		Node->CachedScreenType = InScreenType;
		Node->CachedScreenTitle = InScreenTitle;
		Node->CachedScreenMessage = InScreenMessage;

		Node->RegisterWithGameInstance(World);

		return Node;
	}

	return nullptr;
}

void UAsyncAction_PushConfirmScreen::Activate()
{
	auto FrontendUISubsystem = UFrontendUISubsystem::Get(CachedOwningWorld.Get());
	if (FrontendUISubsystem)
	{
		FrontendUISubsystem->PushConfirmScreenToModalStackAsync(
			CachedScreenType,
			CachedScreenTitle,
			CachedScreenMessage,
			[this](EConfirmScreenButtonType ButtonType)
			{
				OnButtonClicked.Broadcast(ButtonType);

				SetReadyToDestroy();
			});
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FrontendUISubsystem not found!"));
	}
}
