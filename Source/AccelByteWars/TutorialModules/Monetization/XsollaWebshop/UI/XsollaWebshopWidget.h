#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "WebBrowser.h"
#include "Components/Button.h"
#include "Core/UI/AccelByteWarsActivatableWidget.h"
#include "Monetization/XsollaWebshop/XsollaWebshopLog.h"
#include "XsollaWebshopWidget.generated.h"

UCLASS()
class ACCELBYTEWARS_API UXsollaWebshopWidget : public UAccelByteWarsActivatableWidget
{
	GENERATED_BODY()

public:

    /** Storefront base URL used when running with -bUsePreviewStore=true. */
    inline static const FString PreviewWebshopBaseURL = TEXT("https://sitebuilder.xsolla.com/preview/bytewars/");

    /** Storefront base URL used for the published/live store. */
    inline static const FString PublishedWebshopBaseURL = TEXT("https://bytewars.xsolla.site/");

    UFUNCTION(BlueprintCallable, Category = "XsollaWebshop")
    void LoadURL(const FString& NewURL);

    UFUNCTION(BlueprintCallable, Category = "XsollaWebshop")
    void HandleUrlChanged();

protected:

    UPROPERTY(meta = (BindWidget))
    class UWebBrowser* W_WebBrowser;

    UPROPERTY(meta = (BindWidget))
    class UButton* Btn_Close;

    virtual void NativeConstruct() override;
    virtual void NativeOnActivated() override;

    UFUNCTION()
    void OnCloseButtonClicked();

    UFUNCTION()
    void CloseBrowser(const FString& error);

private:
    FString BaseAllowedDomain;

    FString ExtractAndNormalizeDomain(const FString& FullUrl);
};
