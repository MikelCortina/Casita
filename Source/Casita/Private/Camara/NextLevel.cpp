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

    // Partículas portal permanente
    PortalComponent = CreateDefaultSubobject<UPortalComponent>(TEXT("PortalComponent"));

    PortalComponent->SetupAttachment(RootComponent);
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

    // Filtro de clase
    if (!OtherActor->IsA(AMainPlayer::StaticClass()) && !OtherActor->IsA(ACharacter::StaticClass()))
        return;

    if (bTriggerOnce && bAlreadyTriggered) return;

    if (LinkedPortal)
    {
        LinkedPortal->DisableTrigger();

        FVector Destination = LinkedPortal->GetActorLocation();
        OtherActor->SetActorLocation(Destination, false, nullptr, ETeleportType::TeleportPhysics);

        // --- NUEVO: Avisar al jugador que se ha teletransportado ---
        AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
        if (Player)
        {
            Player->ResetPlayerState();
        }

        if (!bTriggerOnce)
        {

        }
    }

    // --- 2. Lógica de Cámara y FX ---
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        if (bUseSplineCamera && CameraManagerRef)
        {
            // Iniciamos el movimiento de cámara
            CameraManagerRef->MoveToNextPoint();

            // ACTIVAR ABSORCIÓN: Justo ahora que la cámara empieza a moverse
            if (PortalComponent)
            {
                PortalComponent->ActivateAbsorption();
            }

            if (LinkedPortal && LinkedPortal->PortalComponent)
            {
                LinkedPortal->PortalComponent->ActivateAbsorption();
            }

            // Iniciar el "Poll" (vigilancia) para detectar cuando la cámara para
            GetWorldTimerManager().SetTimer(
                CameraCheckTimer,
                this,
                &ANextLevel::CheckCameraMovement,
                0.1f, // Revisamos cada 0.1s para mayor precisión
                true
            );
        }
        else if (!bUseSplineCamera && TargetCamera)
        {
            PC->SetViewTargetWithBlend(TargetCamera, BlendTime);
            // Si es cámara estática, sí podemos limpiar aquí
            if (bTriggerOnce) { CleanupAndDestroy(); }
        }
    }

    // IMPORTANTE: bAlreadyTriggered evita que el overlap se ejecute mil veces
    bAlreadyTriggered = true;
}

void ANextLevel::CheckCameraMovement()
{
    if (!CameraManagerRef) return;

    // Si la cámara ya NO se está moviendo...
    if (!CameraManagerRef->IsCameraMoving())
    {
        if (PortalComponent)
        {
            // APAGAR ABSORCIÓN: La cámara se detuvo
            PortalComponent->DeactivateAbsorption();

            // APAGAR PORTAL: Opcional, si quieres que el portal desaparezca al llegar
            PortalComponent->DeactivatePortal();
        }

        // Detener el reloj de vigilancia
        GetWorldTimerManager().ClearTimer(CameraCheckTimer);

        // Si era de un solo uso, ahora es el momento seguro para destruir el actor
        if (bTriggerOnce)
        {
            CleanupAndDestroy();
        }
    }
}

// Función auxiliar para limpiar todo ordenadamente
void ANextLevel::CleanupAndDestroy()
{
    if (PortalComponent) PortalComponent->DestroyFX();
    if (LinkedPortal) LinkedPortal->Destroy();
    Destroy();
}