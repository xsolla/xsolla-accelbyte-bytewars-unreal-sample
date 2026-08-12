// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Auth/XsollaAuth.h"
#include "Access/AuthEssentials/AuthEssentialsModels.h"
#include "Core/AssetManager/TutorialModules/TutorialModuleSubsystem.h"
#include "XsollaAuthSubsystem.generated.h"

struct FXsollaLoginData;

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
	UFUNCTION()
	void OnXsollaLoginSuccess(const FXsollaLoginData LoginData, FLoginUser LoginUser);

	UFUNCTION()
	void OnXsollaLoginCancelled();

	UFUNCTION()
	void OnXsollaLoginFailed(const FString& Description);

private:
	FOnXsollaLoginSuccess OnLoginSuccessDelegate;
	FOnXsollaLoginCancelled OnLoginCancelledDelegate;
	FOnXsollaLoginFailed OnLoginFailedDelegate;

	FAuthOnLoginCompleteDelegate PendingLoginComplete;
};
