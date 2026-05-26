#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UI/InGameUI.h"
#include "CasitaGameMode.generated.h"

UCLASS()
class CASITA_API ACasitaGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    ACasitaGameMode();
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UInGameUI> InGameUIClass;

private:
    UPROPERTY()
    UInGameUI* InGameUIInstance;
};