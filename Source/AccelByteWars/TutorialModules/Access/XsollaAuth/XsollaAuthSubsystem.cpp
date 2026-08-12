// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "XsollaAuthSubsystem.h"
#include "XsollaAuthLog.h"
#include "XsollaAuthModels.h"
#include "XsollaBackendSdkGameSubsystem.h"
#include "Core/XsollaInterfaceManager.h"

void UXsollaAuthSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	OnLoginSuccessDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UXsollaAuthSubsystem, OnXsollaLoginSuccess));
	OnLoginCancelledDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UXsollaAuthSubsystem, OnXsollaLoginCancelled));
	OnLoginFailedDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UXsollaAuthSubsystem, OnXsollaLoginFailed));
}

void UXsollaAuthSubsystem::LoginWithXsolla(const FAuthOnLoginCompleteDelegate& OnLoginComplete)
{
	UXsollaBackendSdkGameSubsystem* XsollaSubsystem = UXsollaBackendSdkGameSubsystem::Get(GetWorld());
	if (!XsollaSubsystem)
	{
		OnLoginComplete.ExecuteIfBound(false, TEXT("Xsolla SDK subsystem is not valid."));
		return;
	}

	PendingLoginComplete = OnLoginComplete;

	UE_LOG_XSOLLAAUTH(Verbose, TEXT("Login with xsolla widget"));
	UXsollaInterfaceManager* InterfaceManager = XsollaSubsystem->GetInterfaceManager();
	InterfaceManager->GetAuth()->AuthWithXsollaWidget(OnLoginSuccessDelegate,
		OnLoginCancelledDelegate,
		OnLoginFailedDelegate);
}

void UXsollaAuthSubsystem::LoginWithSteam(const FAuthOnLoginCompleteDelegate& OnLoginComplete)
{
	UXsollaBackendSdkGameSubsystem* XsollaSubsystem = UXsollaBackendSdkGameSubsystem::Get(GetWorld());
	if (!XsollaSubsystem)
	{
		OnLoginComplete.ExecuteIfBound(false, TEXT("Xsolla SDK subsystem is not valid."));
		return;
	}

	PendingLoginComplete = OnLoginComplete;

	UE_LOG_XSOLLAAUTH(Verbose, TEXT("Silent login with steam"));
	UXsollaInterfaceManager* InterfaceManager = XsollaSubsystem->GetInterfaceManager();

	FOnlineAccountCredentials Credentials;
	Credentials.Type = TEXT("");
	Credentials.Id = TEXT("");
	Credentials.Token = TEXT("");
	InterfaceManager->GetAuth()->SilentSubsystemAuth(STEAM_SUBSYSTEM, Credentials, TEXT(""), OnLoginSuccessDelegate, OnLoginFailedDelegate);
}

void UXsollaAuthSubsystem::OnXsollaLoginSuccess(const FXsollaLoginData LoginData, FLoginUser LoginUser)
{
	UE_LOG_XSOLLAAUTH(Log, TEXT("Xsolla login succes"));
	int LocalUserNum = 0;
	UXsollaAuthModels::SetXsollaLoginData(LocalUserNum, LoginData);
	PendingLoginComplete.ExecuteIfBound(true, TEXT(""));
}

void UXsollaAuthSubsystem::OnXsollaLoginCancelled()
{
	UE_LOG_XSOLLAAUTH(Log, TEXT("Xsolla login cancelled"));
	PendingLoginComplete.ExecuteIfBound(false, TEXT("Login cancelled."));
}

void UXsollaAuthSubsystem::OnXsollaLoginFailed(const FString& Description)
{
	UE_LOG_XSOLLAAUTH(Warning, TEXT("Xsolla failed with error: %s"), *Description);
	PendingLoginComplete.ExecuteIfBound(false, Description);
}
