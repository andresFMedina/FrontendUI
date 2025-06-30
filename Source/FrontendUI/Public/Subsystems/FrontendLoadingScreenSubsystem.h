// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendLoadingScreenSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UFrontendLoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdatedReason, const FString&, InCurrentLoadingReason);
	UPROPERTY(BlueprintAssignable)
	FOnLoadingReasonUpdatedReason OnLoadingReasonUpdated;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;

	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

private:
	bool bIsCurrentlyLoadingMap = false;

	float HoldLoadingScreenStartUpTime = -1;

	FString CurrentLoadingReason;
	
	TSharedPtr<SWidget> CachedCreatedLoadingScreenWidget;

	void OnMapPreloaded(const FWorldContext& WorldContext, const FString&  MapName );

	void OnMapPostLoaded(UWorld* LoadedWorld );

	void TryUpdateLoadingScreen();
	
	bool IsPreLoadScreenActive() const;

	bool ShouldShowLoadingScreen();

	bool IsLoadingScreenNeeded();

	void TryDisplayLoadingScreenIfNone();

	void TryRemoveLoadingScreen();

	void NotifyLoadingScreenVisibilityChanged(bool bIsVisible);
};
