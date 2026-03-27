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

    PortalComponent = CreateDefaultSubobject<UPortalComponent>(TEXT("PortalComponent"));

    // Si no haces esto, el componente flota en el vacío (0,0,0)
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

void ANextLevel::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OverlappedComponent || !OtherActor) return;

    if (!OtherActor->IsA(AMainPlayer::StaticClass()) && !OtherActor->IsA(ACharacter::StaticClass()))
        return;

    if (bTriggerOnce && bAlreadyTriggered) return;

    // IMPORTANTE: Marcamos que ya se activó y desactivamos el trigger 
    // para que no haya teletransportes infinitos mientras la cámara viaja.
    bAlreadyTriggered = true;
    DisableTrigger();

    // --- Teleport ---
    if (LinkedPortal)
    {
        LinkedPortal->DisableTrigger();
        FVector Destination = LinkedPortal->GetActorLocation();
        OtherActor->SetActorLocation(Destination, false, nullptr, ETeleportType::TeleportPhysics);
    }

    // --- Cámara ---
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        if (bUseSplineCamera && CameraManagerRef)
        {
            if (CameraManagerRef->IsCameraMoving())
            {
                CameraManagerRef->DisableInput();
            }
            else
            {
                CameraManagerRef->MoveToNextPoint();
            }

            if (PortalComponent)
                PortalComponent->ActivateAbsorption();

            // Iniciamos el timer que revisa cuándo se para la cámara
            GetWorldTimerManager().SetTimer(CameraCheckTimer, this, &ANextLevel::CheckCameraMovement, 0.2f, true);
        }
        else if (!bUseSplineCamera && TargetCamera)
        {
            PC->SetViewTargetWithBlend(TargetCamera, BlendTime);

        }
    }

}

void ANextLevel::CheckCameraMovement()
{
    if (!CameraManagerRef) return;

    // Si la cámara ya terminó de moverse...
    if (!CameraManagerRef->IsCameraMoving())
    {
        // 1. Apagar efectos visuales
        if (PortalComponent)
            PortalComponent->DeactivateAbsorption();

        // 2. Limpiar el timer para que deje de ejecutar esta función
        GetWorldTimerManager().ClearTimer(CameraCheckTimer);

        // 3. Lógica de destrucción diferida
        if (bTriggerOnce)
        {
            // Limpiar partículas antes de borrar
            if (PortalComponent)
                PortalComponent->DestroyFX();

            if (LinkedPortal)
            {
                // También limpiamos las partículas del portal pareja si las tiene
                if (LinkedPortal->PortalComponent)
                    LinkedPortal->PortalComponent->DestroyFX();

                LinkedPortal->Destroy();
            }

            // Finalmente, nos destruimos a nosotros mismos
            Destroy();
        }
    }
}