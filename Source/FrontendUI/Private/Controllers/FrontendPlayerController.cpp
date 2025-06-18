// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/FrontendPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

void AFrontendPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	TArray<AActor*> CameraActors;

	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("Default"), CameraActors);

	if (!CameraActors.IsEmpty())
	{
		AActor* CameraActor = CameraActors[0];
		if (CameraActor)
		{
			SetViewTargetWithBlend(CameraActor);
		}
	}

	auto FrontendSettings = UFrontendGameUserSettings::Get();

	if (FrontendSettings->GetLastCPUBenchmarkResult() == -1.f || FrontendSettings->GetLastGPUBenchmarkResult() == -1.f)
	{
		FrontendSettings->RunHardwareBenchmark();
		FrontendSettings->ApplyHardwareBenchmarkResults();
	}

}
