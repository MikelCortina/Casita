#include "Camara/NextLevel.h"
#include "Components/BoxComponent.h"
#include "Player/MainPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ANextLevel::ANextLevel()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;
    TriggerBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
    TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    TriggerBox->SetGenerateOverlapEvents(true);
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ANextLevel::BeginPlay()
{
    Super::BeginPlay();

    if (TriggerBox)
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ANextLevel::OnOverlapBegin);

    CameraManagerRef = Cast<ACameraManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ACameraManager::StaticClass()));
}

void ANextLevel::DisableTrigger()
{
    if (TriggerBox)
    {
        TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        TriggerBox->SetGenerateOverlapEvents(false);
    }
    bAlreadyTriggered = true;
}

void ANextLevel::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OverlappedComponent || !OtherActor) return;

    // Solo reacciona al jugador
    if (!OtherActor->IsA(AMainPlayer::StaticClass()) &&
        !OtherActor->IsA(ACharacter::StaticClass()))
        return;

    // Disparo único
    if (bTriggerOnce && bAlreadyTriggered) return;

    // --- Teleport al centro del portal pareja ---
    if (LinkedPortal)
    {
        // Desactiva el portal pareja temporalmente para evitar
        // que el teleport dispare su overlap inmediatamente
        LinkedPortal->DisableTrigger();

        // Teletransporta al jugador al centro exacto del otro trigger
        FVector Destination = LinkedPortal->GetActorLocation();
        OtherActor->SetActorLocation(Destination, false, nullptr,
            ETeleportType::TeleportPhysics);

        // Reactivar el portal pareja después de un frame
        // (por si quieres que el sistema sea reutilizable y bTriggerOnce = false)
        // Si bTriggerOnce = true ambos se destruyen igualmente abajo
        if (!bTriggerOnce)
        {
            FTimerHandle TimerHandle;
            GetWorldTimerManager().SetTimer(TimerHandle,
                [this]()
                {
                    if (LinkedPortal)
                        LinkedPortal->bAlreadyTriggered = false;
                },
                0.5f, false);
        }
    }

    // --- Cámara ---
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        if (bUseSplineCamera && CameraManagerRef)
        {
            // Si la cámara ya está en movimiento, desactiva el input
            if (CameraManagerRef->IsCameraMoving())
            {
                CameraManagerRef->DisableInput();
            }
            else
            {
                // Si no está en movimiento, inicia el movimiento (que desactiva input automáticamente)
                CameraManagerRef->MoveToNextPoint();
            }
        }
        else if (!bUseSplineCamera && TargetCamera)
            PC->SetViewTargetWithBlend(TargetCamera, BlendTime);
    }

    // --- Destruir ambos portales si es de un solo uso ---
    if (bTriggerOnce)
    {
        if (LinkedPortal)
            LinkedPortal->Destroy();

        Destroy();
    }
}