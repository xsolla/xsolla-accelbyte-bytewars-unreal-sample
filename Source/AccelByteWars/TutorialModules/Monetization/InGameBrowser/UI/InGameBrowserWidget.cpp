// Fill out your copyright notice in the Description page of Project Settings.

#include "Monetization/InGameBrowser/UI/InGameBrowserWidget.h"
#include "Monetization/InGameBrowser/InGameBrowserSubsystem.h"

DEFINE_LOG_CATEGORY(LogInGameBrowser);

void UInGameBrowserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (WBP_WebBrowserComponent)
    {
        WBP_WebBrowserComponent->LoadURL("https://xsolla.com/");
    }

    if (Btn_Close)
    {
        Btn_Close->OnClicked.RemoveAll(this);
        Btn_Close->OnClicked.AddDynamic(this, &UInGameBrowserWidget::OnCloseButtonClicked);
    }

    FScriptDelegate ScriptDelegate;
    ScriptDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UInGameBrowserWidget, HandleUrlChanged));

    WBP_WebBrowserComponent->OnUrlChanged.RemoveAll(this);
    WBP_WebBrowserComponent->OnUrlChanged.AddUnique(ScriptDelegate);

}

void UInGameBrowserWidget::LoadURL(const FString& NewURL)
{
    if (WBP_WebBrowserComponent)
    {
        WBP_WebBrowserComponent->LoadURL(NewURL);
    }
}

void UInGameBrowserWidget::HandleUrlChanged()
{
    // Find the Subsystem and tell it to close the browser (which handles input resetting).
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UInGameBrowserSubsystem* BrowserSubsystem = GameInstance->GetSubsystem<UInGameBrowserSubsystem>())
        {
            FString CurrentUrl = WBP_WebBrowserComponent->GetUrl();
            UE_LOG(LogInGameBrowser, Log, TEXT("URL Changed To : %s"), *CurrentUrl);

            BrowserSubsystem->HandleUrlChanged(CurrentUrl);
        }
    }
}

void UInGameBrowserWidget::OnCloseButtonClicked()
{
    CloseBrowser("");
}

void UInGameBrowserWidget::CloseBrowser(const FString& error)
{
    // Find the Subsystem and tell it to close the browser (which handles input resetting).
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UInGameBrowserSubsystem* BrowserSubsystem = GameInstance->GetSubsystem<UInGameBrowserSubsystem>())
        {
            BrowserSubsystem->CloseBrowser(error);
        }
    }
}
