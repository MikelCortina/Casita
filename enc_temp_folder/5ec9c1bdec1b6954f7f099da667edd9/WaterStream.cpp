#include "Components/WaterStream.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Player/MainPlayer.h" 

AWaterStream::AWaterStream()
{
    PrimaryActorTick.bCanEverTick = false; // No necesitamos Tick aquí, ahorramos rendimiento

    // 1. Crear el volumen del trigger y hacerlo la raíz
    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    RootComponent = TriggerVolume;

    // Ajustar el perfil para que detecte solapamientos pero no bloquee físicamente
    TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
    TriggerVolume->SetGenerateOverlapEvents(true);
    // Un tamaño por defecto (puedes estirarlo en el editor)
    TriggerVolume->InitBoxExtent(FVector(100.0f, 100.0f, 300.0f));

    // 2. Crear una malla visual opcional
    WaterVisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterVisualMesh"));
    WaterVisualMesh->SetupAttachment(RootComponent);
    // La malla visual NO debe tener colisión, de eso se encarga el TriggerVolume
    WaterVisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 3. Vincular los eventos de solapamiento a nuestras funciones
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
    // Comprobar que el otro actor existe y no es este mismo objeto
    if (OtherActor && OtherActor != this)
    {
        // Intentar castear (convertir) el otro actor a nuestra clase MainPlayer
        AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
        if (Player)
        {
            // Entró el jugador: Multiplicamos su gravedad
            Player->GravityFactor *= GravityMultiplier;
            UE_LOG(LogTemp, Warning, TEXT("¡La flor entró al agua! Gravedad aumentada a: %f"), Player->GravityFactor);
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
            UE_LOG(LogTemp, Warning, TEXT("¡La flor salió del agua! Gravedad normalizada a: %f"), Player->GravityFactor);
        }
    }
}