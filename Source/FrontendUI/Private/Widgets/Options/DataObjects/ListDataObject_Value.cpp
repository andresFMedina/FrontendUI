// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_Value.h"

void UListDataObject_Value::SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InGetterFuncPath)
{
	DataDynamicGetter = InGetterFuncPath;
}

void UListDataObject_Value::SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InSetterFuncPath)
{
	DataDynamicSetter = InSetterFuncPath;
}
