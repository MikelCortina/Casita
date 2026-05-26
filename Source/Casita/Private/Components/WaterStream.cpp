#include "Components/WaterStream.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Player/MainPlayer.h" 

DEFINE_LOG_CATEGORY(LogWaterStream);

AWaterStream::AWaterStream()
{
    PrimaryActorTick.bCanEverTick = false; 
    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    RootComponent = TriggerVolume;

    TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
    TriggerVolume->SetGenerateOverlapEvents(true);
    TriggerVolume->InitBoxExtent(FVector(100.0f, 100.0f, 300.0f));

    WaterVisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterVisualMesh"));
    WaterVisualMesh->SetupAttachment(RootComponent);
    WaterVisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AWaterStream::OnOverlapBegin);
    TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AWaterStream::OnOverlapEnd);

    LowWaterParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LowWaterParticles"));
    LowWaterParticles->SetupAttachment(RootComponent);
}

void AWaterStream::BeginPlay()
{
    Super::BeginPlay();
}

void AWaterStream::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
        if (Player)
        {
            Player->GravityFactor *= GravityMultiplier;
            UE_LOG(LogWaterStream, Warning, TEXT("¡La flor entró al agua! Gravedad aumentada a: %f"), Player->GravityFactor);
        }
    }
}

void AWaterStream::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this)
    {
        AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
        if (Player)
        {
            // Salió el jugador: Restauramos su gravedad original dividiendo
            Player->GravityFactor /= GravityMultiplier;
            UE_LOG(LogWaterStream, Warning, TEXT("¡La flor salió del agua! Gravedad normalizada a: %f"), Player->GravityFactor);
        }
    }
}