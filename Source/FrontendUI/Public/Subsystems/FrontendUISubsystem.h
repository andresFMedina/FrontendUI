// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendUISubsystem.generated.h"

class UWidget_PrimaryLayout;
class UWidget_ActivatableBase;
struct FGameplayTag;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush,
};

UCLASS()
class FRONTENDUI_API UFrontendUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UFrontendUISubsystem* Get(const UObject* WorldContextObject);
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayout(UWidget_PrimaryLayout* InCreatedWidget);

	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
		TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback);

private:
	UWidget_PrimaryLayout* CreatedPrimaryLayout;
};
