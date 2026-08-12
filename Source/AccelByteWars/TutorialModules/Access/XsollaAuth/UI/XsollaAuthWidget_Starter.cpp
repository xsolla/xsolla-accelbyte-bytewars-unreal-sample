#include "XsollaAuthWidget_Starter.h"
#include "Access/XsollaAuth/XsollaAuthLog.h"
#include "Access/XsollaAuth/XsollaAuthSubsystem_Starter.h"
#include "Core/UI/AccelByteWarsBaseUI.h"
#include "Core/UI/Components/AccelByteWarsButtonBase.h"
#include "Access/AuthEssentials/UI/LoginWidget_Starter.h"
#include "Access/SinglePlatformAuth/UI/SinglePlatformAuthWidget_Starter.h"

void UXsollaAuthWidget_Starter::NativeConstruct()
{
	Super::NativeConstruct();

	const FPrimaryAssetId AuthEssentialsAssetId = FPrimaryAssetId("TutorialModule:AUTHESSENTIALS");
	AuthEssentialsModule = UTutorialModuleUtility::GetTutorialModuleDataAsset(AuthEssentialsAssetId, this);
	ensure(AuthEssentialsModule);

	ParentWidget = UAccelByteWarsBaseUI::GetActiveWidgetOfStack(EBaseUIStackType::Menu, this);
	ensure(ParentWidget);

	LoginWidget = Cast<ULoginWidget_Starter>(ParentWidget);
	if (!LoginWidget)
	{
		return;
	}

	AuthSubsystem = GetGameInstance()->GetSubsystem<UXsollaAuthSubsystem_Starter>();
	ensure(AuthSubsystem);
}

void UXsollaAuthWidget_Starter::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (!AuthEssentialsModule->IsStarterModeActive())
	{
		UE_LOG_XSOLLAAUTH(Warning, TEXT("AuthEssentials is not in starter mode and this is the starter variant of xsolla login. Please also enable the starter mode for the xsolla login module."))
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
			if (USinglePlatformAuthWidget_Starter* SinglePlatformAuthWidget = Cast<USinglePlatformAuthWidget_Starter>(Widget))
			{
				SinglePlatformAuthWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		});
	}
}

void UXsollaAuthWidget_Starter::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	Btn_LoginWithXsolla->OnClicked().Clear();
	Btn_LoginWithSteamXsolla->OnClicked().Clear();
}

void UXsollaAuthWidget_Starter::OnLoginWithXsollaButtonClicked()
{
	// TODO: Implement login with Xsolla widget here.
	UE_LOG_XSOLLAAUTH(Warning, TEXT("Login with Xsolla widget is not yet implemented."));
}

void UXsollaAuthWidget_Starter::OnLoginWithSteamButtonClicked()
{
	// TODO: Implement silent login with Steam via Xsolla here.
	UE_LOG_XSOLLAAUTH(Warning, TEXT("Silent login with Steam via Xsolla is not yet implemented."));
}

void UXsollaAuthWidget_Starter::OnLoginComplete(bool bWasSuccessful, const FString& ErrorMessage)
{
	// TODO: Handle on login complete event.
	UE_LOG_XSOLLAAUTH(Warning, TEXT("On login complete event is not yet implemented."));
}
