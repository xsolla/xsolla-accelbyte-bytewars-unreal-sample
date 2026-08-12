#include "XsollaAuthWidget.h"
#include "Access/XsollaAuth/XsollaAuthLog.h"
#include "Access/XsollaAuth/XsollaAuthSubsystem.h"
#include "Core/UI/AccelByteWarsBaseUI.h"
#include "Core/UI/Components/AccelByteWarsButtonBase.h"
#include "Access/AuthEssentials/UI/LoginWidget.h"
#include "Access/SinglePlatformAuth/UI/SinglePlatformAuthWidget.h"

void UXsollaAuthWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FPrimaryAssetId AuthEssentialsAssetId = FPrimaryAssetId("TutorialModule:AUTHESSENTIALS");
	AuthEssentialsModule = UTutorialModuleUtility::GetTutorialModuleDataAsset(AuthEssentialsAssetId, this);
	ensure(AuthEssentialsModule);

	ParentWidget = UAccelByteWarsBaseUI::GetActiveWidgetOfStack(EBaseUIStackType::Menu, this);
	ensure(ParentWidget);

	LoginWidget = Cast<ULoginWidget>(ParentWidget);
	if (!LoginWidget)
	{
		return;
	}

	AuthSubsystem = GetGameInstance()->GetSubsystem<UXsollaAuthSubsystem>();
	ensure(AuthSubsystem);
}

void UXsollaAuthWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (AuthEssentialsModule->IsStarterModeActive())
	{
		UE_LOG_XSOLLAAUTH(Warning, TEXT("AuthEssentials is in starter mode and xsolla login is dependent upon it. Please also disable the starter mode for the xsolla login module."))
		Btn_LoginWithXsolla->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	Btn_LoginWithXsolla->OnClicked().AddUObject(this, &ThisClass::OnLoginWithXsollaButtonClicked);

	const bool bIsSteamActive = IOnlineSubsystem::Get(STEAM_SUBSYSTEM) != nullptr;
	if (!bIsSteamActive)
	{
		Btn_LoginWithSteamXsolla->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Btn_LoginWithSteamXsolla->OnClicked().AddUObject(this, &ThisClass::OnLoginWithSteamButtonClicked);
	}

	LoginWidget->SetButtonLoginVisibility(ESlateVisibility::Collapsed);

	if (LoginWidget->WidgetTree)
	{
		LoginWidget->WidgetTree->ForEachWidget([](UWidget* Widget)
		{
			if (USinglePlatformAuthWidget* SinglePlatformAuthWidget = Cast<USinglePlatformAuthWidget>(Widget))
			{
				SinglePlatformAuthWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		});
	}
}

void UXsollaAuthWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	Btn_LoginWithXsolla->OnClicked().Clear();
	Btn_LoginWithSteamXsolla->OnClicked().Clear();
}

void UXsollaAuthWidget::OnLoginWithXsollaButtonClicked()
{
	LoginWidget->SetLoginState(ELoginState::LoggingIn);
	LoginWidget->OnRetryLoginDelegate.AddUObject(this, &ThisClass::OnLoginWithXsollaButtonClicked);
	AuthSubsystem->LoginWithXsolla(FAuthOnLoginCompleteDelegate::CreateUObject(this, &ThisClass::OnLoginComplete));
}

void UXsollaAuthWidget::OnLoginWithSteamButtonClicked()
{
	LoginWidget->SetLoginState(ELoginState::LoggingIn);
	LoginWidget->OnRetryLoginDelegate.AddUObject(this, &ThisClass::OnLoginWithSteamButtonClicked);
	AuthSubsystem->LoginWithSteam(FAuthOnLoginCompleteDelegate::CreateUObject(this, &ThisClass::OnLoginComplete));
}

void UXsollaAuthWidget::OnLoginComplete(bool bWasSuccessful, const FString& ErrorMessage)
{
	if (!bWasSuccessful)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { LoginWidget->SetLoginState(ELoginState::Failed); }, 2.0f, false);
	}

	LoginWidget->OnLoginComplete(bWasSuccessful, ErrorMessage);
}
