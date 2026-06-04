#include "WaterStream/Valve.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/MainPlayer.h"
#include "WaterStream/HardWaterStream.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

AValve::AValve()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    Mesh->SetMobility(EComponentMobility::Movable);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ProximityTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ProximityTrigger"));
    ProximityTrigger->SetupAttachment(RootComponent);
    ProximityTrigger->SetSphereRadius(200.0f);
    ProximityTrigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
    ProximityTrigger->OnComponentBeginOverlap.AddDynamic(this, &AValve::OnProximityBeginOverlap);
    ProximityTrigger->OnComponentEndOverlap.AddDynamic(this, &AValve::OnProximityEndOverlap);
}

void AValve::BeginPlay()
{
    Super::BeginPlay();
}

void AValve::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bSpinning) return;

    AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
    SpinTimer += DeltaTime;

    if (SpinTimer >= SpinDuration)
    {
        bSpinning = false;
        SpinTimer = 0.f;
    }
}


void AValve::ShowInteractPrompt()
{
    if (InteractPromptWidget || !InteractPromptClass) return;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    InteractPromptWidget = CreateWidget<UUserWidget>(PC, InteractPromptClass);
    if (InteractPromptWidget)
        InteractPromptWidget->AddToViewport();
}

void AValve::HideInteractPrompt()
{
    if (InteractPromptWidget)
    {
        InteractPromptWidget->RemoveFromParent();
        InteractPromptWidget = nullptr;
    }
}


void AValve::TryInteract()
{
    if (!bPlayerInRange || bActivated) return;

    bActivated = true;
    bSpinning = true;
    HideInteractPrompt();

    if (LinkedWaterStream)
    {
        LinkedWaterStream->DeactivateStream();
    }
}


void AValve::OnProximityBeginOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AMainPlayer* Player = Cast<AMainPlayer>(OtherActor))
    {
        bPlayerInRange = true;
        Player->NearbyValve = this;
        ShowInteractPrompt();
    }
}

void AValve::OnProximityEndOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (AMainPlayer* Player = Cast<AMainPlayer>(OtherActor))
    {
        bPlayerInRange = false;
        Player->NearbyValve = nullptr;
        HideInteractPrompt();
    }
}