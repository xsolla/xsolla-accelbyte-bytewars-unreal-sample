// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Auth/XsollaAccelByteAuth.h"
#include "Access/AuthEssentials/AuthEssentialsModels.h"
#include "Core/AssetManager/TutorialModules/TutorialModuleSubsystem.h"
#include "XsollaAuthSubsystem.generated.h"

UCLASS()
class ACCELBYTEWARS_API UXsollaAuthSubsystem : public UTutorialModuleSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Login using the Xsolla widget (login/registration form). */
	void LoginWithXsolla(const FAuthOnLoginCompleteDelegate& OnLoginComplete);

	/** Silently login using the Steam platform via Xsolla. */
	void LoginWithSteam(const FAuthOnLoginCompleteDelegate& OnLoginComplete);

protected:
	void OnXsollaLoginSuccess(const FXsollaAccelByteLoginResult& LoginResult);

	void OnXsollaLoginCancelled();

	void OnXsollaLoginFailed(const FString& Code, const FString& Description);

private:
	FOnXsollaAccelByteLoginSuccess OnLoginSuccessDelegate;
	FOnXsollaAccelByteLoginCancelled OnLoginCancelledDelegate;
	FOnXsollaAccelByteLoginFailed OnLoginFailedDelegate;

	FAuthOnLoginCompleteDelegate PendingLoginComplete;
};
