#pragma once


namespace Debug 
{
	static void Print(const FString& Message, int32 InKey = -1, float Duration = 7.0f, FColor Color = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, Duration, Color, Message);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
		}
	}
}