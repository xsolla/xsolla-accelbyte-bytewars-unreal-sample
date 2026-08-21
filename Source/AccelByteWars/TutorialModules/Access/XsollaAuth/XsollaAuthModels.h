// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Auth/XsollaAccelByteAuthTypes.h"
#include "XsollaAuthModels.generated.h"

UCLASS()
class ACCELBYTEWARS_API UXsollaAuthModels : public UObject
{
	GENERATED_BODY()

public:
	static FXsollaAccelByteLoginResult* GetXsollaLoginData(int32 LocalUserNum) { return XsollaLoginData.Find(LocalUserNum); }
	static void SetXsollaLoginData(int32 LocalUserNum, FXsollaAccelByteLoginResult InXsollaLoginData) { XsollaLoginData.Add(LocalUserNum, InXsollaLoginData); }

private:
	inline static TMap<int32, FXsollaAccelByteLoginResult> XsollaLoginData;
};
