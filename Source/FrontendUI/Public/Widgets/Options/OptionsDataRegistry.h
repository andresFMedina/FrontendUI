// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OptionsDataRegistry.generated.h"


class UListDataObject_Collection;
class UListDataObject_Base;
/**
 * 
 */
UCLASS()
class FRONTENDUI_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()

public:

	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	FORCEINLINE const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabsCollections() const { return RegisteredOptionsTabsCollections; }

	TArray<UListDataObject_Base*> GetListSourceItemsBySelectedTabId(const FName& TabID) const;
	

private:

	UPROPERTY(Transient)
	TArray<UListDataObject_Collection*> RegisteredOptionsTabsCollections;

	void InitGameplayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlsCollectionTab();
	
};
