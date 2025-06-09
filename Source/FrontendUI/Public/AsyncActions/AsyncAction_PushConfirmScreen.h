// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "AsyncAction_PushConfirmScreen.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmScreenButtonClicked, EConfirmScreenButtonType, ButtonType);
/**
 *
 */
UCLASS()
class FRONTENDUI_API UAsyncAction_PushConfirmScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Show Confirmation Screen"))
	static UAsyncAction_PushConfirmScreen* ShowConfirmationScreen(
		const UObject* WorldContextObject,
		EConfirmScreenType InScreenType,
		FText InScreenTitle,
		FText InScreenMessage
	);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable, Category = "AsyncAction")
	FOnConfirmScreenButtonClicked OnButtonClicked;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	EConfirmScreenType CachedScreenType;
	FText CachedScreenTitle;
	FText CachedScreenMessage;
};
