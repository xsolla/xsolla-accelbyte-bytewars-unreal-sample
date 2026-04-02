
#include "Monetization/InGameBrowser/InGameBrowserSubsystem.h"

#if !UE_BUILD_SHIPPING 
static TAutoConsoleVariable<bool> CVarABIngameBrowserDisableExternalUrlDeflector(
    TEXT("ab.ingamebrowser.disableExternalUrlDeflector"),
    false,
    TEXT("Deflect the url access in the In Game Browser when domain different with the initial access"),
    ECVF_Default
);
#endif

UInGameBrowserSubsystem::UInGameBrowserSubsystem()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(
        TEXT("/Game/TutorialModules/Monetization/InGameBrowser/UI/WB_InGameBrowser.WB_InGameBrowser_C")
    );

    if (WidgetClassFinder.Succeeded())
    {
        TSoftClassPtr<UUserWidget> BrowserWidgetSoftClass;
        BrowserWidgetSoftClass = WidgetClassFinder.Class;
        InGameBrowserWidget = BrowserWidgetSoftClass.LoadSynchronous();
    }
    else
    {
        // This is a warning, as the C++ default still needs the asset to exist.
        UE_LOG(LogTemp, Error, TEXT("FATAL: Failed to find WB_InGameBrowser_C. Check the path and _C suffix."));
    }
}

void UInGameBrowserSubsystem::OpenBrowser(const FString& InitialURL, UWidget* CurrentWidget, const FOnBrowserClosed& OnClosedCallback)
{
    OnBrowserClosedDelegate = OnClosedCallback;

    LastWidgetOpened = CurrentWidget;

    CreateBrowserWidget(); // Ensure the instance exists

    APlayerController* PC = GWorld->GetFirstPlayerController();

    if (BrowserWidgetInstance && PC)
    {
        if (!BrowserWidgetInstance->IsInViewport())
        {
            BrowserWidgetInstance->AddToViewport(100);

            FInputModeGameAndUI InputMode;
            InputMode.SetWidgetToFocus(BrowserWidgetInstance->TakeWidget());
            PC->SetInputMode(InputMode);
            PC->SetShowMouseCursor(true);
        }

        BrowserWidgetInstance->LoadURL(InitialURL);
    }
}

void UInGameBrowserSubsystem::CloseBrowser(const FString& error)
{
    if (BrowserWidgetInstance && BrowserWidgetInstance->IsInViewport())
    {
        BrowserWidgetInstance->RemoveFromParent();

        APlayerController* PC = GWorld->GetFirstPlayerController();
        if (PC)
        {
            FInputModeUIOnly InputMode;
            if (LastWidgetOpened)
            {
                InputMode.SetWidgetToFocus(LastWidgetOpened->TakeWidget());
            }
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
            LastWidgetOpened = nullptr;
        }

        if (OnBrowserClosedDelegate.IsBound())
        {
            OnBrowserClosedDelegate.Execute(error);

            // Clear the delegate
            OnBrowserClosedDelegate.Unbind();
        }
    }
}

void UInGameBrowserSubsystem::HandleUrlChanged(const FString& Url)
{
    const FString& CurrentUrl = Url;

    // CAPTURE THE BASE DOMAIN (Only on the first change)
    if (BaseAllowedDomain.IsEmpty())
    {
        BaseAllowedDomain = ExtractAndNormalizeDomain(CurrentUrl);
        UE_LOG(LogInGameBrowser, Log, TEXT("Browser initialized. Base Allowed Domain set to: %s"), *BaseAllowedDomain);

        return;
    }

    FString CurrentDomain = ExtractAndNormalizeDomain(CurrentUrl);

    if (CurrentDomain.Contains(BaseAllowedDomain))
    {
        UE_LOG(LogInGameBrowser, Log, TEXT("URL Change Allowed: %s"), *CurrentUrl);
    }
    else
    {
        FString ErrorMessage = FString::Printf(TEXT("External link detected! to: %s.\n\n Expected domain: %s"), *CurrentUrl, *BaseAllowedDomain);

#if !UE_BUILD_SHIPPING 
        if (CVarABIngameBrowserDisableExternalUrlDeflector.GetValueOnAnyThread())
        {
            UE_LOG(LogInGameBrowser, Log, TEXT("External Url Deflector is disabled, Allow all domain"));
            return;
        }
#endif

        // EXTERNAL URL DETECTED: Trigger closure.
        UE_LOG(LogInGameBrowser, Warning, TEXT("%s"), *ErrorMessage);

        CloseBrowser(ErrorMessage);
    }
}

FString UInGameBrowserSubsystem::ExtractAndNormalizeDomain(const FString& FullUrl)
{
    FString CleanUrl = FullUrl;

    CleanUrl = CleanUrl.ToLower();
    CleanUrl.ReplaceInline(TEXT("https://"), TEXT(""), ESearchCase::CaseSensitive);
    CleanUrl.ReplaceInline(TEXT("http://"), TEXT(""), ESearchCase::CaseSensitive);

    int32 Index;

    if (CleanUrl.FindChar(TCHAR('?'), Index)) CleanUrl = CleanUrl.Left(Index);

    // Find Fragment Start ('#') and truncate
    if (CleanUrl.FindChar(TCHAR('#'), Index)) CleanUrl = CleanUrl.Left(Index);

    // Find Path Start ('/') and truncate
    if (CleanUrl.FindChar(TCHAR('/'), Index)) CleanUrl = CleanUrl.Left(Index);

    // 2. Remove "www." prefix
    CleanUrl.ReplaceInline(TEXT("www."), TEXT(""), ESearchCase::CaseSensitive);

    return CleanUrl;
}

void UInGameBrowserSubsystem::CreateBrowserWidget()
{
    if (BrowserWidgetInstance) return; // Already created

    if (!InGameBrowserWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("FATAL: No browser widget class could be loaded. Check WidgetClassFinder implementation."));
        return;
    }

    APlayerController* PC = GWorld->GetFirstPlayerController();
    if (PC)
    {
        BrowserWidgetInstance = CreateWidget<UInGameBrowserWidget>(PC, InGameBrowserWidget);
    }
}
