// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_String.generated.h"

class UFrontendCommonButtonBase;
class UFrontendCommonRotator;
class UMyListDataObject_String;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDUI_API UWidget_ListEntry_String : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void OnOwningListDataViewSet(UListDataObject_Base* InOwningDataObject) override;

	virtual void OnOwningListDataObjectModified(UListDataObject_Base* InOwningDataObject, EOptionsListDataModifyReason ModifyReason) override;

	virtual void OnToggleEditableState(bool bIsEditable) override;


private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess="true"))
	UFrontendCommonButtonBase* CommonButton_Previous;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonRotator* CommonRotator_AvailableOptions;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_Next;

	UPROPERTY(Transient)
	UMyListDataObject_String* OwningStringDataObject;

	void OnPreviousOptionButtonClicked();
	void OnNextOptionButtonClicked();
	void OnRotatorValueChanged(int32 Value, bool bUserInitated);
};
