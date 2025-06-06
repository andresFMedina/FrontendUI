// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "AsyncAction_PushSoftWidget.generated.h"

class UWidget_ActivatableBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetCompleted, UWidget_ActivatableBase*, PushedWidget);

UCLASS()
class FRONTENDUI_API UAsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push Soft Widget To Widget Stack"))
	static UAsyncAction_PushSoftWidget* PushSoftWidget(
		const UObject* WorldContextObject,
		APlayerController* OwningPlayerController,
		TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
		UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag InWidgetTag,
		bool bFocusOnNewlyPushedWidget = true
	);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable, Category = "AsyncAction")
	FOnPushSoftWidgetCompleted OnWidgetCreatedBeforePushed;

	UPROPERTY(BlueprintAssignable, Category = "AsyncAction")
	FOnPushSoftWidgetCompleted AfterPushed;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedOwningPlayerController;
	TSoftClassPtr<UWidget_ActivatableBase> CachedSoftWidgetClass;
	FGameplayTag CachedWidgetTag;
	bool bCachedFocusOnNewlyPushedWidget = false;
	
};
