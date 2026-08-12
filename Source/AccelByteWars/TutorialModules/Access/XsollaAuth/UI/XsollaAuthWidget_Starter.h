#pragma once

#include "CoreMinimal.h"
#include "Access/AuthEssentials/AuthEssentialsModels.h"
#include "Core/UI/AccelByteWarsActivatableWidget.h"
#include "XsollaAuthWidget_Starter.generated.h"

class ULoginWidget_Starter;
class UAccelByteWarsButtonBase;
class UXsollaAuthSubsystem_Starter;

#define AUTH_ESSENTIALS_SECTION_STARTER TEXT("/ByteWars/TutorialModule.AuthEssentials")
#define XSOLLA_LOGIN_SECTION_STARTER TEXT("/ByteWars/TutorialModule.XsollaLoginWidget")

UCLASS(Abstract)
class ACCELBYTEWARS_API UXsollaAuthWidget_Starter : public UAccelByteWarsActivatableWidget
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
	ULoginWidget_Starter* LoginWidget;

	UPROPERTY()
	UXsollaAuthSubsystem_Starter* AuthSubsystem;

	void OnLoginComplete(bool bWasSuccessful, const FString& ErrorMessage);
};
