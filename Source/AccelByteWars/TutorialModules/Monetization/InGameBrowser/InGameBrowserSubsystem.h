#pragma once


#include "CoreMinimal.h"
#include "UI/InGameBrowserWidget.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InGameBrowserSubsystem.generated.h"

class UBrowserWidget;
class UUserWidget;

DECLARE_DELEGATE_OneParam(FOnBrowserClosed, const FString&);

UCLASS(Blueprintable, BlueprintType)
class ACCELBYTEWARS_API UInGameBrowserSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    UInGameBrowserSubsystem();

    void OpenBrowser(const FString& InitialURL, UWidget* CurrentWidget, const FOnBrowserClosed& OnClosedCallback);

    void CloseBrowser(const FString& error = "");

    UPROPERTY(EditAnywhere, Category = "InGameBrowser")
    TSubclassOf<UUserWidget> InGameBrowserWidget = nullptr;

    UFUNCTION(BlueprintCallable, Category = "InGameBrowser")
    void HandleUrlChanged(const FString& Url);

private:
    UPROPERTY()
    UInGameBrowserWidget* BrowserWidgetInstance = nullptr;

    FString BaseAllowedDomain;

    UWidget* LastWidgetOpened;

    FOnBrowserClosed OnBrowserClosedDelegate;

    FString ExtractAndNormalizeDomain(const FString& FullUrl);

    void CreateBrowserWidget();
};