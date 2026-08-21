#include "Monetization/XsollaWebshop/UI/XsollaWebshopWidget.h"
#include "Access/XsollaAuth/XsollaAuthModels.h"
#include "Core/UI/Components/Prompt/PromptSubsystem.h"

#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<bool> CVarABXsollaWebshopDisableExternalUrlDeflector(
    TEXT("ab.xsollawebshop.disableExternalUrlDeflector"),
    false,
    TEXT("Deflect the url access in the Xsolla Webshop browser when domain different with the initial access"),
    ECVF_Default
);
#endif

void UXsollaWebshopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_Close)
    {
        Btn_Close->OnClicked.RemoveAll(this);
        Btn_Close->OnClicked.AddDynamic(this, &UXsollaWebshopWidget::OnCloseButtonClicked);
    }

    if (W_WebBrowser)
    {
        FScriptDelegate ScriptDelegate;
        ScriptDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UXsollaWebshopWidget, HandleUrlChanged));

        W_WebBrowser->OnUrlChanged.RemoveAll(this);
        W_WebBrowser->OnUrlChanged.AddUnique(ScriptDelegate);
    }
}

void UXsollaWebshopWidget::NativeOnActivated()
{
    Super::NativeOnActivated();

    BaseAllowedDomain.Empty();

    const FXsollaAccelByteLoginResult* LoginData = UXsollaAuthModels::GetXsollaLoginData(0);
    if (!LoginData)
    {
        UE_LOG(LogXsollaWebshop, Warning, TEXT("Cannot open webshop. No cached Xsolla login data for local user 0."));
        DeactivateWidget();
        return;
    }

    bool bUsePreviewStore = false;
    bool bUseExternalBrowser = false;
    FParse::Bool(FCommandLine::Get(), TEXT("-bUsePreviewStore="), bUsePreviewStore);
    FParse::Bool(FCommandLine::Get(), TEXT("-bUseExternalBrowser="), bUseExternalBrowser);

    const FString& BaseURL = bUsePreviewStore ? PreviewWebshopBaseURL : PublishedWebshopBaseURL;
    const FString URL = FString::Printf(TEXT("%s?token=%s"), *BaseURL, *LoginData->XsollaAccessToken);

    if (bUseExternalBrowser)
    {
        FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
        DeactivateWidget();
        return;
    }

    LoadURL(URL);
}

void UXsollaWebshopWidget::LoadURL(const FString& NewURL)
{
    if (W_WebBrowser)
    {
        W_WebBrowser->LoadURL(NewURL);
    }
}

void UXsollaWebshopWidget::HandleUrlChanged()
{
    if (!W_WebBrowser)
    {
        return;
    }

    const FString CurrentUrl = W_WebBrowser->GetUrl();
    UE_LOG(LogXsollaWebshop, Log, TEXT("URL Changed To : %s"), *CurrentUrl);

    if (BaseAllowedDomain.IsEmpty())
    {
        BaseAllowedDomain = ExtractAndNormalizeDomain(CurrentUrl);
        UE_LOG(LogXsollaWebshop, Log, TEXT("Browser initialized. Base Allowed Domain set to: %s"), *BaseAllowedDomain);
        return;
    }

    const FString CurrentDomain = ExtractAndNormalizeDomain(CurrentUrl);

    if (CurrentDomain.Contains(BaseAllowedDomain))
    {
        UE_LOG(LogXsollaWebshop, Log, TEXT("URL Change Allowed: %s"), *CurrentUrl);
        return;
    }

    const FString ErrorMessage = FString::Printf(TEXT("External link detected! to: %s.\n\n Expected domain: %s"), *CurrentUrl, *BaseAllowedDomain);

#if !UE_BUILD_SHIPPING
    if (CVarABXsollaWebshopDisableExternalUrlDeflector.GetValueOnAnyThread())
    {
        UE_LOG(LogXsollaWebshop, Log, TEXT("External Url Deflector is disabled, Allow all domain"));
        return;
    }
#endif

    UE_LOG(LogXsollaWebshop, Warning, TEXT("%s"), *ErrorMessage);

    CloseBrowser(ErrorMessage);
}

FString UXsollaWebshopWidget::ExtractAndNormalizeDomain(const FString& FullUrl)
{
    FString CleanUrl = FullUrl;

    CleanUrl = CleanUrl.ToLower();
    CleanUrl.ReplaceInline(TEXT("https://"), TEXT(""), ESearchCase::CaseSensitive);
    CleanUrl.ReplaceInline(TEXT("http://"), TEXT(""), ESearchCase::CaseSensitive);

    int32 Index;

    if (CleanUrl.FindChar(TCHAR('?'), Index)) CleanUrl = CleanUrl.Left(Index);
    if (CleanUrl.FindChar(TCHAR('#'), Index)) CleanUrl = CleanUrl.Left(Index);
    if (CleanUrl.FindChar(TCHAR('/'), Index)) CleanUrl = CleanUrl.Left(Index);

    CleanUrl.ReplaceInline(TEXT("www."), TEXT(""), ESearchCase::CaseSensitive);

    return CleanUrl;
}

void UXsollaWebshopWidget::OnCloseButtonClicked()
{
    CloseBrowser("");
}

void UXsollaWebshopWidget::CloseBrowser(const FString& error)
{
    if (!error.IsEmpty())
    {
        if (UGameInstance* GameInstance = GetGameInstance())
        {
            if (UPromptSubsystem* PromptSubsystem = GameInstance->GetSubsystem<UPromptSubsystem>())
            {
                PromptSubsystem->ShowDialoguePopUp(
                    ERROR_PROMPT_TEXT,
                    FText::FromString(error),
                    EPopUpType::MessageOk,
                    FPopUpResultDelegate::CreateWeakLambda(this, [](EPopUpResult Result) {})
                );
            }
        }
    }

    DeactivateWidget();
}
