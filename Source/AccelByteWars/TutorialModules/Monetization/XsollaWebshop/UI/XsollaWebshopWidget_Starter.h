#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "WebBrowser.h"
#include "Components/Button.h"
#include "Core/UI/AccelByteWarsActivatableWidget.h"
#include "Monetization/XsollaWebshop/XsollaWebshopLog.h"
#include "XsollaWebshopWidget_Starter.generated.h"

UCLASS()
class ACCELBYTEWARS_API UXsollaWebshopWidget_Starter : public UAccelByteWarsActivatableWidget
{
	GENERATED_BODY()

public:
    // TODO: Add your public function declarations here.

protected:
    UPROPERTY(meta = (BindWidget))
    class UWebBrowser* W_WebBrowser;

    UPROPERTY(meta = (BindWidget))
    class UButton* Btn_Close;

    // TODO: Add your protected function declarations here.

private:
    // TODO: Add your private function declarations here.
};
