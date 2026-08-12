#pragma once

#include "CoreMinimal.h"
#include "Access/AuthEssentials/AuthEssentialsModels.h"
#include "Core/UI/AccelByteWarsActivatableWidget.h"
#include "XsollaAuthWidget.generated.h"

class ULoginWidget;
class UAccelByteWarsButtonBase;
class UXsollaAuthSubsystem;

#define AUTH_ESSENTIALS_SECTION TEXT("/ByteWars/TutorialModule.AuthEssentials")
#define XSOLLA_LOGIN_SECTION TEXT("/ByteWars/TutorialModule.XsollaLoginWidget")

UCLASS(Abstract)
class ACCELBYTEWARS_API UXsollaAuthWidget : public UAccelByteWarsActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

private:
	void OnLoginWithXsollaButtonClicked();
	void OnLoginWithSteamButtonClicked();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	UAccelByteWarsButtonBase* Btn_LoginWithXsolla;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	UAccelByteWarsButtonBase* Btn_LoginWithSteamXsolla;

	UPROPERTY()
	UTutorialModuleDataAsset* AuthEssentialsModule;

	UPROPERTY()
	UCommonActivatableWidget* ParentWidget;

	UPROPERTY()
	ULoginWidget* LoginWidget;

	UPROPERTY()
	UXsollaAuthSubsystem* AuthSubsystem;

	void OnLoginComplete(bool bWasSuccessful, const FString& ErrorMessage);
};
