// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Value.generated.h"

class FOptionsDataInteractionHelper;
/**
 * 
 */
UCLASS(Abstract)
class FRONTENDUI_API UListDataObject_Value : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InGetterFuncPath);
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InSetterFuncPath);
	
	
protected:
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;
};
