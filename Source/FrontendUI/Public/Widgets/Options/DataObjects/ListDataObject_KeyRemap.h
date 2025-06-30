// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "CommonInputTypeEnum.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ListDataObject_KeyRemap.generated.h"

class UEnhancedInputUserSettings;
class UEnhancedPlayerMappableKeyProfile;
/**
 * 
 */
UCLASS()
class FRONTENDUI_API UListDataObject_KeyRemap : public UListDataObject_Base
{
	GENERATED_BODY()
public:
	void InitKeyRemapData(UEnhancedInputUserSettings* InOwningInputUserSettings, UEnhancedPlayerMappableKeyProfile* InKeyProfile, ECommonInputType InInputTypeKey, const FPlayerKeyMapping& InOwningPlayerKeyMapping);

	FSlateBrush GetIconFromCurrentKey()const;

	virtual bool HasDefaultValue() const override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	
private:	

	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedOwningInputUserSettings;
	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedOwningKeyProfile;
	
	ECommonInputType CachedDesiredInputKeyType;	
	FName CachedOwningMappingName;	
	EPlayerMappableKeySlot CachedOwningMappableKeySlot;

	FPlayerKeyMapping* GetOwningKeyMapping() const;

public:
	FORCEINLINE ECommonInputType GetDesiredInputType() const { return CachedDesiredInputKeyType; }
	void BindNewInputKey(const FKey& InNewKey);
};
