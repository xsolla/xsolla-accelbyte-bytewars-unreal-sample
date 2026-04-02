#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Blueprint/UserWidget.h"
#include "WebBrowser.h"
#include "Components/Button.h"
#include "InGameBrowserWidget.generated.h"

/**
 * 
 */

DECLARE_LOG_CATEGORY_EXTERN(LogInGameBrowser, Log, All); 

UCLASS()
class ACCELBYTEWARS_API UInGameBrowserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

    UFUNCTION(BlueprintCallable, Category = "InGameBrowser")
    void LoadURL(const FString& NewURL);

    UFUNCTION(BlueprintCallable, Category = "InGameBrowser")
    void HandleUrlChanged();

protected:

    UPROPERTY(meta = (BindWidget))
    class UWebBrowser* WBP_WebBrowserComponent;

    UPROPERTY(meta = (BindWidget))
    class UButton* Btn_Close;

    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnCloseButtonClicked();

    UFUNCTION()
    void CloseBrowser(const FString& error);
};
