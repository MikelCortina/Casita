#include "UI/CasitaGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ACasitaGameMode::ACasitaGameMode()
{
    InGameUIInstance = nullptr;
}

void ACasitaGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (InGameUIClass)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC)
        {
            InGameUIInstance = CreateWidget<UInGameUI>(PC, InGameUIClass);
            if (InGameUIInstance)
            {
                InGameUIInstance->AddToViewport();
            }
        }
    }
}