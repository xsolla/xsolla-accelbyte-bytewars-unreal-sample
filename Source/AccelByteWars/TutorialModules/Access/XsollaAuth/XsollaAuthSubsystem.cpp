// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "XsollaAuthSubsystem.h"
#include "XsollaAuthLog.h"
#include "XsollaAuthModels.h"
#include "XsollaAccelByteSubsystem.h"

void UXsollaAuthSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	OnLoginSuccessDelegate.BindUObject(this, &UXsollaAuthSubsystem::OnXsollaLoginSuccess);
	OnLoginCancelledDelegate.BindUObject(this, &UXsollaAuthSubsystem::OnXsollaLoginCancelled);
	OnLoginFailedDelegate.BindUObject(this, &UXsollaAuthSubsystem::OnXsollaLoginFailed);
}

void UXsollaAuthSubsystem::LoginWithXsolla(const FAuthOnLoginCompleteDelegate& OnLoginComplete)
{
	UXsollaAccelByteSubsystem* XsollaSubsystem = UXsollaAccelByteSubsystem::Get(GetWorld());
	if (!XsollaSubsystem)
	{
		OnLoginComplete.ExecuteIfBound(false, TEXT("Xsolla SDK subsystem is not valid."));
		return;
	}

	PendingLoginComplete = OnLoginComplete;

	UE_LOG_XSOLLAAUTH(Verbose, TEXT("Login with xsolla widget"));
	XsollaSubsystem->GetAuth()->LoginWithXsollaAccount(OnLoginSuccessDelegate,
		OnLoginCancelledDelegate,
		OnLoginFailedDelegate);
}

void UXsollaAuthSubsystem::LoginWithSteam(const FAuthOnLoginCompleteDelegate& OnLoginComplete)
{
	UXsollaAccelByteSubsystem* XsollaSubsystem = UXsollaAccelByteSubsystem::Get(GetWorld());
	if (!XsollaSubsystem)
	{
		OnLoginComplete.ExecuteIfBound(false, TEXT("Xsolla SDK subsystem is not valid."));
		return;
	}

	PendingLoginComplete = OnLoginComplete;

	UE_LOG_XSOLLAAUTH(Verbose, TEXT("Silent login with steam"));

	FOnlineAccountCredentials Credentials;
	Credentials.Type = TEXT("");
	Credentials.Id = TEXT("");
	Credentials.Token = TEXT("");
	XsollaSubsystem->GetAuth()->LoginWithXsollaSilentAuth(STEAM_SUBSYSTEM, Credentials, TEXT(""), OnLoginSuccessDelegate, OnLoginFailedDelegate);
}

void UXsollaAuthSubsystem::OnXsollaLoginSuccess(const FXsollaAccelByteLoginResult& LoginResult)
{
	UE_LOG_XSOLLAAUTH(Log, TEXT("Xsolla login succes"));
	UXsollaAuthModels::SetXsollaLoginData(LoginResult.LocalUserNum, LoginResult);
	PendingLoginComplete.ExecuteIfBound(true, TEXT(""));
}

void UXsollaAuthSubsystem::OnXsollaLoginCancelled()
{
	UE_LOG_XSOLLAAUTH(Log, TEXT("Xsolla login cancelled"));
	PendingLoginComplete.ExecuteIfBound(false, TEXT("Login cancelled."));
}

void UXsollaAuthSubsystem::OnXsollaLoginFailed(const FString& Code, const FString& Description)
{
	UE_LOG_XSOLLAAUTH(Warning, TEXT("Xsolla failed with error: %s"), *Description);
	PendingLoginComplete.ExecuteIfBound(false, Description);
}
