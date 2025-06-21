#pragma once


#include "FrontendStructTypes.generated.h"

USTRUCT()
struct FOptionsDataEditConditionDescriptor
{
	GENERATED_BODY()

public:
	void SetEditConditionFunc(const TFunction<bool()>& InEditConditionFunc)
	{
		EditConditionFunct = InEditConditionFunc;
	}
	bool IsValid() const
	{
		return EditConditionFunct != nullptr;
	}
	bool IsConditionMet() const
	{
		if (IsValid())
		{
			return EditConditionFunct();
		}

		return true;
	}

	FString GetDisabledRichReason() const
	{
		return DisabledRichReason;
	}

	void SetDisabledRichReason(const FString& InDisabledRichReason)
	{
		DisabledRichReason = InDisabledRichReason;
	}

	bool HasForcedStringValue() const
	{
		return DisabledForcedStringValue.IsSet();
	}

	FString GetForcedStringValue() const
	{
		return DisabledForcedStringValue.GetValue();
	}

	void SetForcedStringValue(const FString& InForcedStringValue)
	{
		DisabledForcedStringValue = InForcedStringValue;
	}

private:
	TFunction<bool()> EditConditionFunct;
	FString DisabledRichReason;
	TOptional<FString> DisabledForcedStringValue;
};