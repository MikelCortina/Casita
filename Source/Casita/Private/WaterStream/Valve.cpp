#include "WaterStream/Valve.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/MainPlayer.h"
#include "WaterStream/HardWaterStream.h"
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
        UE_LOG(LogTemp, Warning, TEXT("Válvula deja de girar"));
    }
}

void AValve::TryInteract()
{
    if (!bPlayerInRange || bActivated) return;

    bActivated = true;
    bSpinning = true;   

    if (LinkedWaterStream)
    {
        LinkedWaterStream->DeactivateStream();
        UE_LOG(LogTemp, Warning, TEXT("Llave girada — agua cerrada"));
    }
}

void AValve::OnProximityBeginOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AMainPlayer* Player = Cast<AMainPlayer>(OtherActor))
    {
        bPlayerInRange = true;
        Player->SetNearbyValve(this);
        UE_LOG(LogTemp, Warning, TEXT("Jugador cerca de la llave — pulsa Interact"));
    }
}

void AValve::OnProximityEndOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (AMainPlayer* Player = Cast<AMainPlayer>(OtherActor))
    {
        bPlayerInRange = false;
        Player->ClearNearbyValve();
    }
}