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
    int32                OtherBodyIndex,
    bool                 bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OverlappedComponent || !OtherActor) return;

    if (!OtherActor->IsA(AMainPlayer::StaticClass()) && !OtherActor->IsA(ACharacter::StaticClass()))
        return;

    if (bTriggerOnce && bAlreadyTriggered) return;

    if (LinkedPortal)
    {
        LinkedPortal->DisableTrigger();

        FVector Destination = LinkedPortal->GetActorLocation();
        OtherActor->SetActorLocation(Destination, false, nullptr, ETeleportType::TeleportPhysics);

        AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
        if (Player)
            Player->ResetPlayerState();
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        if (bUseSplineCamera && CameraManagerRef)
        {
            CameraManagerRef->MoveToNextPoint();

            if (PortalComponent)
                PortalComponent->ActivateAbsorption();

            if (LinkedPortal && LinkedPortal->PortalComponent)
                LinkedPortal->PortalComponent->ActivateAbsorption();

            GetWorldTimerManager().SetTimer(
                CameraCheckTimer,
                this,
                &ANextLevel::CheckCameraMovement,
                0.1f,
                true);
        }
        else if (!bUseSplineCamera && TargetCamera)
        {
            PC->SetViewTargetWithBlend(TargetCamera, BlendTime);

            if (bTriggerOnce)
                CleanupAndDestroy();
        }
    }

    bAlreadyTriggered = true;
}

void ANextLevel::CheckCameraMovement()
{
    if (!CameraManagerRef) return;

    if (!CameraManagerRef->IsCameraMoving())
    {
        if (PortalComponent)
        {
            PortalComponent->DeactivateAbsorption();
            PortalComponent->DeactivatePortal();
        }

        GetWorldTimerManager().ClearTimer(CameraCheckTimer);

        if (bTriggerOnce)
            CleanupAndDestroy();
    }
}

void ANextLevel::CleanupAndDestroy()
{
    if (PortalComponent)  PortalComponent->DestroyFX();
    if (LinkedPortal)     LinkedPortal->Destroy();
    Destroy();
}