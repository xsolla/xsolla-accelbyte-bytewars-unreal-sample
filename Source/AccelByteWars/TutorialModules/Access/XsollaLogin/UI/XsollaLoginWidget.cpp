#include "XsollaLoginWidget.h"
#include "Access/XsollaLogin/XsollaLoginLog.h"
#include "Core/UI/AccelByteWarsBaseUI.h"
#include "Core/UI/Components/AccelByteWarsButtonBase.h"
#include "Access/AuthEssentials/AuthEssentialsSubsystem.h"
#include "Access/AuthEssentials/UI/LoginWidget.h"

#include "XsollaBackendSdkGameSubsystem.h"
#include "Core/XsollaInterfaceManager.h"
#include <TutorialModuleUtilities/TutorialModuleOnlineUtility.h>

void UXsollaLoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FPrimaryAssetId AuthEssentialsAssetId = FPrimaryAssetId("TutorialModule:AUTHESSENTIALS");
	AuthEssentialsModule = UTutorialModuleUtility::GetTutorialModuleDataAsset(AuthEssentialsAssetId, this);
	ensure(AuthEssentialsModule);

	ParentWidget = UAccelByteWarsBaseUI::GetActiveWidgetOfStack(EBaseUIStackType::Menu, this);
	ensure(ParentWidget);

	AuthSubsystem = GetGameInstance()->GetSubsystem<UAuthEssentialsSubsystem>();
	LoginWidget = Cast<ULoginWidget>(ParentWidget);
	if (!LoginWidget || !AuthSubsystem)
	{
		/*
		 * This could happen if AuthEssentials is in starter mode.
		 * There is already a checker on NativeOnActivated, simply return here.
		 */
		return;
	}
	OnLoginSucces.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UXsollaLoginWidget, OnLoginSuccess));
	OnLoginCancel.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UXsollaLoginWidget, OnLoginCanceled));
	OnLoginFail.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UXsollaLoginWidget, OnLoginFailed));
}

void UXsollaLoginWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (AuthEssentialsModule->IsStarterModeActive())
	{
		UE_LOG_AUTH_ESSENTIALS(Warning, TEXT("AuthEssentials is in starter mode and xsolla login is dependent upon it. Please also disable the starter mode for the xsolla login module."))
		Btn_XsollaLoginWithWidget->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	Btn_XsollaLoginWithWidget->SetButtonText(TEXT_LOGIN_WITH);
	Btn_XsollaLoginWithWidget->OnClicked().AddUObject(this, &ThisClass::OnXsollaLoginWithWidgetButtonClicked);
	Btn_XsollaLoginWithSteam->OnClicked().AddUObject(this, &ThisClass::OnXsollaLoginWithSteamButtonClicked);
	LoginWidget->SetButtonLoginVisibility(ESlateVisibility::Collapsed);
}

void UXsollaLoginWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	Btn_XsollaLoginWithWidget->OnClicked().Clear();
}

void UXsollaLoginWidget::OnXsollaLoginWithWidgetButtonClicked()
{
	UXsollaBackendSdkGameSubsystem* XsollaSubsystem = UXsollaBackendSdkGameSubsystem::Get(GetWorld());
	if(XsollaSubsystem)
	{
		UE_LOG_XSOLLALOGIN(Verbose, TEXT("Login with xsolla widget"));
		UXsollaInterfaceManager* InterfaceManager = XsollaSubsystem->GetInterfaceManager();
		InterfaceManager->GetAuth()->AuthWithXsollaWidget(OnLoginSucces,
			OnLoginCancel,
			OnLoginFail);
	}
}

void UXsollaLoginWidget::OnXsollaLoginWithSteamButtonClicked()
{
	UXsollaBackendSdkGameSubsystem* XsollaSubsystem = UXsollaBackendSdkGameSubsystem::Get(GetWorld());
	if(XsollaSubsystem)
	{
		UE_LOG_XSOLLALOGIN(Verbose, TEXT("Silent login with steam"));
		UXsollaInterfaceManager* InterfaceManager = XsollaSubsystem->GetInterfaceManager();

		FOnlineAccountCredentials Credentials;
		Credentials.Type = TEXT("");
		Credentials.Id = TEXT("");
		Credentials.Token = TEXT("");
		InterfaceManager->GetAuth()->SilentSubsystemAuth(STEAM_SUBSYSTEM, Credentials, TEXT(""), OnLoginSucces,	OnLoginFail);
		LoginWidget->SetLoginState(ELoginState::LoggingIn);
	}
}

void UXsollaLoginWidget::OnLoginCanceled()
{
	UE_LOG_XSOLLALOGIN(Log, TEXT("Xsolla login cancelled"));
}

void UXsollaLoginWidget::OnLoginFailed(const FString& Description)
{
	UE_LOG_XSOLLALOGIN(Warning, TEXT("Xsolla failed with error: %s"), *Description);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { LoginWidget->SetLoginState(ELoginState::Failed); }, 2.0f, false);

	AuthSubsystem->OnLoginComplete(0, false, *FUniqueNetIdAccelByteUser::Invalid(), Description, FAuthOnLoginCompleteDelegate::CreateUObject(LoginWidget, &std::remove_pointer_t<decltype(LoginWidget)>::OnLoginComplete));
}

void UXsollaLoginWidget::OnLoginSuccess(const FXsollaLoginData LoginData, FLoginUser LoginUser)
{
	UE_LOG_XSOLLALOGIN(Log, TEXT("Xsolla login succes"));
	int LocalUserNum = 0;
	UTutorialModuleOnlineUtility::SetXsollaLoginData(LocalUserNum, LoginData);
	AuthSubsystem->OnLoginComplete(LoginUser.LocalUserNum, true, *LoginUser.UserNetId, TEXT(""), FAuthOnLoginCompleteDelegate::CreateUObject(LoginWidget, &std::remove_pointer_t<decltype(LoginWidget)>::OnLoginComplete));
}

