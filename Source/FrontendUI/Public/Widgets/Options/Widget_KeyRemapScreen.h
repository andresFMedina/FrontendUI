// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "CommonInputBaseTypes.h"
#include "Widget_KeyRemapScreen.generated.h"

class UCommonRichTextBlock;
class FKeyRemapScreenInputPreprocesor;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDUI_API UWidget_KeyRemapScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

public:
	void SetDesiredInputType(ECommonInputType InDesiredInputType);

	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeyPressedDelegate, const FKey& /*PressedKey*/)
	FOnKeyRemapScreenKeyPressedDelegate OnKeyRemapScreenKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenSelectCanceledDelegate, const FString& /*CanceledReason*/)
	FOnKeyRemapScreenSelectCanceledDelegate OnKeyRemapScreenSelectCanceled;

protected:
	virtual void NativeOnActivated() override;

	virtual void NativeOnDeactivated() override;

private:
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichText_RemapMessage;

	TSharedPtr<FKeyRemapScreenInputPreprocesor> CachedInputPreprocessor;

	ECommonInputType CachedDesiredInputType;

	void OnInputKeyPressed(const FKey& PressedKey);
	void OnInputKeyCanceled(const FString& CanceledReason);
	
	//Delay a tick to make sure the input is captured
	void RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback);
};
