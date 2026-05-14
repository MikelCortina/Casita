#include "Level/EndTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

AEndTrigger::AEndTrigger()
{
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEndTrigger::OnOverlapBegin);
}

void AEndTrigger::BeginPlay()
{
    Super::BeginPlay();
}

void AEndTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (bTriggered || !OtherActor) return;

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    // Comprobar que es el jugador
    if (OtherActor != PC->GetPawn()) return;

    bTriggered = true;

    if (EndScreenWidgetClass)
    {
        UUserWidget* Widget = CreateWidget<UUserWidget>(PC, EndScreenWidgetClass);
        if (Widget)
        {
            Widget->AddToViewport();
        }
    }

    // Pausar el juego y mostrar cursor
    PC->SetIgnoreMoveInput(true);
    PC->SetIgnoreLookInput(true);
    PC->bShowMouseCursor = true;
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}