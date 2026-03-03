#include "Player/MainPlayer.h"
#include "Components/ParticulasComponent.h"
#include "Components/InputComponent.h"

AMainPlayer::AMainPlayer()
{
    PrimaryActorTick.bCanEverTick = true;
    ParticulasComponent = CreateDefaultSubobject<UParticulasComponent>(TEXT("ParticulasComponent"));

    GrowDirection = FVector::UpVector;
    LastAnchorPosition = FVector::ZeroVector;
    CurrentSurfaceNormal = FVector::UpVector;
}

void AMainPlayer::BeginPlay()
{
    Super::BeginPlay();
}

void AMainPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (FMath::IsNearlyZero(InputForward) && FMath::IsNearlyZero(InputRight)) return;

    // 1. DEFINIR DIRECCIÓN DE MOVIMIENTO EN EL MUNDO (Sin importar la pared)
    // Usamos los ejes globales para que el control sea intuitivo para el jugador
    FVector WorldMoveDir = (FVector::ForwardVector * InputForward) + (FVector::RightVector * InputRight);
    WorldMoveDir.Normalize();

    // 2. PROYECTAR EL MOVIMIENTO SOBRE EL PLANO DE LA PARED ACTUAL
    // Esto hace que si intentas ir "hacia adelante" contra una pared, el vector se convierta en "hacia arriba"
    FVector ProjectedDir = FVector::VectorPlaneProject(WorldMoveDir, CurrentSurfaceNormal).GetSafeNormal();

    // 3. DETECCIÓN DE COLISIÓN Y ADHERENCIA (Sweep de Esfera)
    FHitResult Hit;
    FVector Start = GetActorLocation();
    FVector StepVector = ProjectedDir * GrowSpeed * DeltaTime;

    // Intentamos movernos, pero inclinando el rayo un poco hacia la superficie para no perder el contacto
    FVector End = Start + StepVector + (-CurrentSurfaceNormal * 10.0f);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    FCollisionShape Sphere = FCollisionShape::MakeSphere(tipRadius);

    // Ejecutamos el barrido para detectar la pared
    if (GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_Visibility, Sphere, Params))
    {
        // Actualizamos la normal de la superficie (para el siguiente frame)
        CurrentSurfaceNormal = Hit.Normal;

        // POSICIONAMIENTO: Forzamos la ubicación en el punto de impacto + el radio
        // NO tocamos la rotación (SetActorRotation), por lo que se queda bloqueada.
        FVector NewLocation = Hit.Location + (CurrentSurfaceNormal * tipRadius);
        SetActorLocation(NewLocation, true);
    }
    else
    {
        // 4. BÚSQUEDA EN ESQUINAS (Si el suelo desaparece bajo nosotros)
        FVector DownCheck = Start + StepVector + (-CurrentSurfaceNormal * StickDistance);
        if (GetWorld()->SweepSingleByChannel(Hit, Start + StepVector, DownCheck, FQuat::Identity, ECC_Visibility, Sphere, Params))
        {
            CurrentSurfaceNormal = Hit.Normal;
            SetActorLocation(Hit.Location + (CurrentSurfaceNormal * tipRadius), true);
        }
        else
        {
            // Movimiento simple si no hay nada cerca
            SetActorLocation(Start + StepVector, true);
        }
    }
}

// --- ESTO ES LO QUE FALTABA PARA EVITAR EL ERROR DE COMPILACIÓN ---
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);
    PlayerInputComponent->BindAction("SpawnParticles", IE_Pressed, this, &AMainPlayer::ActivateParticles);
}

void AMainPlayer::MoveForward(float Value) { InputForward = Value; }
void AMainPlayer::MoveRight(float Value) { InputRight = Value; }

void AMainPlayer::AnclarEnFlor(FVector FlorPos)
{
    LastAnchorPosition = FlorPos;
    bTieneUltimaAncla = true;
}

void AMainPlayer::ActivateParticles()
{
    if (ParticulasComponent)
    {
        ParticulasComponent->SpawnParticles();
    }
}