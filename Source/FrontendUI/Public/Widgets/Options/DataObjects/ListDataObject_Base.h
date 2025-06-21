// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "ListDataObject_Base.generated.h"


#define LIST_DATA_ACCESSOR(Datatype, PropertyName) \
FORCEINLINE Datatype Get##PropertyName() const { return PropertyName; } \
void Set##PropertyName(Datatype In##PropertyName) { PropertyName = In##PropertyName; } 

UCLASS(Abstract)
class FRONTENDUI_API UListDataObject_Base : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, UListDataObject_Base*, EOptionsListDataModifyReason);
	FOnListDataModifiedDelegate OnListDataModified;
	FOnListDataModifiedDelegate OnDependencyDataModified;

	LIST_DATA_ACCESSOR(FName, DataID);
	LIST_DATA_ACCESSOR(FText, DataDisplayName);
	LIST_DATA_ACCESSOR(FText, DescriptionRichText);
	LIST_DATA_ACCESSOR(FText, DisabledRichText);
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage);
	LIST_DATA_ACCESSOR(UListDataObject_Base*, ParentData);

	void InitDataObject();

	//Empty in the base
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const { return TArray<UListDataObject_Base*>(); }
	virtual bool HasAnyChildListData() const { return false; }

	FORCEINLINE void SetShouldApplyChangesImmediatly(bool bInShouldApplyChangesImmediatly) { bShouldApplyChangesImmediatly = bInShouldApplyChangesImmediatly; }

	virtual bool HasDefaultValue() const { return false; }
	virtual bool CanResetBackToDefaultValue() const { return false; }
	virtual bool TryResetBackToDefaultValue() { return false; }

	void AddEditCondition(const FOptionsDataEditConditionDescriptor& InEditCondition);

	void AddEditDependencyData(UListDataObject_Base* InDependencyData);

	bool IsDataCurrentlyEditable();



protected:
	virtual void OnDataObjectInitialized();
	virtual void NotifyListDataModified(UListDataObject_Base* InDataModified, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified);

	virtual bool CanSetToForcedStringValue(const FString& InForcedValue) const { return false; }
	virtual void OnSetToForcedStringValue(const FString& InForcedValue) {}

	virtual void OnEditDependencyDataModified(UListDataObject_Base* InDependencyDataModified, EOptionsListDataModifyReason ModifyReason);

private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;

	UPROPERTY(Transient)
	UListDataObject_Base* ParentData;

	bool bShouldApplyChangesImmediatly = false;

	UPROPERTY(Transient)
	TArray<FOptionsDataEditConditionDescriptor> EditContionDescArray;
};
