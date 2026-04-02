#pragma once

#include "CoreMinimal.h"
#include "Auth/XsollaAuth.h"
#include "Core/UI/AccelByteWarsActivatableWidget.h"
#include "XsollaLoginWidget.generated.h"

class ULoginWidget;
class UAccelByteWarsButtonBase;
class UAuthEssentialsSubsystem;
struct FXsollaLoginData;

#define TEXT_LOGIN_WITH NSLOCTEXT("AccelByteWars", "login_with", "Login Xsolla")
#define AUTH_ESSENTIALS_SECTION TEXT("/ByteWars/TutorialModule.AuthEssentials")
#define XSOLLA_LOGIN_SECTION TEXT("/ByteWars/TutorialModule.XsollaLoginWidget")

UCLASS(Abstract)
class ACCELBYTEWARS_API UXsollaLoginWidget : public UAccelByteWarsActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	private:
	void OnXsollaLoginWithWidgetButtonClicked();
	void OnXsollaLoginWithSteamButtonClicked();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	UAccelByteWarsButtonBase* Btn_XsollaLoginWithWidget;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	UAccelByteWarsButtonBase* Btn_XsollaLoginWithSteam;

	UPROPERTY()
	UTutorialModuleDataAsset* AuthEssentialsModule;

	UPROPERTY()
	UCommonActivatableWidget* ParentWidget;

	UPROPERTY()
	ULoginWidget* LoginWidget;

	UPROPERTY()
	UAuthEssentialsSubsystem* AuthSubsystem;

	UFUNCTION()
	
	void OnLoginCanceled();
	UFUNCTION()
	void OnLoginFailed(const FString& Description);
	
	UFUNCTION()
	void OnLoginSuccess(const FXsollaLoginData LoginData, FLoginUser LoginUser);

	FOnXsollaLoginSuccess OnLoginSucces;
	FOnXsollaLoginCancelled OnLoginCancel;
	FOnXsollaLoginFailed OnLoginFail;
};