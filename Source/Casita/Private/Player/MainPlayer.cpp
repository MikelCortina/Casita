#include "Player/MainPlayer.h"
#include "Components/ParticulasComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
// --- NUEVO: Incluir la librería del componente instanciado ---
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

AMainPlayer::AMainPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    // Colisión de caja como Raíz
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->InitBoxExtent(FVector(32.0f, 32.0f, 32.0f));
    CollisionBox->SetCollisionProfileName(TEXT("Pawn"));
    // Importante: Bloquear todo para que no atraviese nada
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    RootComponent = CollisionBox;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // --- NUEVO: Configuración del InstancedTrailComponent ---
    InstancedTrailComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedTrail"));
    InstancedTrailComponent->SetupAttachment(RootComponent);
    InstancedTrailComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // Para que las instancias no se muevan cuando el jugador avanza, 
    // desligamos el componente de la posición y rotación del Root:
    InstancedTrailComponent->SetAbsolute(true, true, true);
    // --------------------------------------------------------

    ParticulasComponent = CreateDefaultSubobject<UParticulasComponent>(TEXT("ParticulasComponent"));
}

void AMainPlayer::BeginPlay()
{
    Super::BeginPlay();

    // Establecemos el punto de inicio como el primer checkpoint automáticamente
    LastCheckpointLocation = GetActorLocation();
    MaxDistanceReached = 0.0f;
    bHasCheckpoint = true;

    // --- NUEVO: Inicializamos la posición de la estela ---
    LastTrailLocation = GetActorLocation();
}

void AMainPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector DeltaMovement = FVector::ZeroVector;
    bool bIsTryingToMove = (InputForward != 0.0f || InputRight != 0.0f);

    // --- 1. MOVIMIENTO DE INPUT ---
    if (bIsTryingToMove)
    {
        APlayerController* PC = Cast<APlayerController>(GetController());
        if (PC && PC->PlayerCameraManager)
        {
            FRotator CameraRotation = PC->PlayerCameraManager->GetCameraRotation();
            FVector CameraRight = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);
            FVector CameraUp = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Z);

            FVector MovementDirection = (CameraUp * InputForward) + (CameraRight * InputRight);

            if (!MovementDirection.IsNearlyZero())
            {
                MovementDirection.Normalize();
                DeltaMovement = MovementDirection * MoveSpeed * DeltaTime;
            }
        }
    }

    // --- 2. GRAVEDAD ACUMULATIVA (Solo si hay Input) ---
    if (bHasCheckpoint && bIsTryingToMove)
    {
        // Calculamos distancia actual al último checkpoint
        float CurrentDistance = FVector::Dist(GetActorLocation(), LastCheckpointLocation);

        if (CurrentDistance > MaxDistanceReached)
        {
            MaxDistanceReached = CurrentDistance;
        }

        // La fuerza de caída solo se calcula y aplica si el jugador se está moviendo
        float VerticalFall = MaxDistanceReached * GravityFactor * DeltaTime;
        DeltaMovement.Z -= VerticalFall;
    }

    // --- 3. APLICAR MOVIMIENTO FINAL ---
    if (!DeltaMovement.IsNearlyZero())
    {
        FHitResult Hit;
        // El barrido (sweep) es true para que choque con el suelo/paredes
        AddActorWorldOffset(DeltaMovement, true, &Hit);

        // Deslizamiento simple
        if (Hit.IsValidBlockingHit())
        {
            FVector SlideVector = FVector::VectorPlaneProject(DeltaMovement, Hit.Normal);
            if (SlideVector.Size() > 0.01f)
            {
                AddActorWorldOffset(SlideVector, true);
            }
        }
    }

    // --- 4. NUEVO: GENERAR LA RAMA A SU PASO ---
    float DistanceSinceLastTrail = FVector::Dist(GetActorLocation(), LastTrailLocation);

    if (DistanceSinceLastTrail >= SpawnTrailDistance)
    {
        FTransform SpawnTransform;
        SpawnTransform.SetLocation(GetActorLocation());

        // --- NUEVO: Le aplicamos la escala que hayas configurado ---
        SpawnTransform.SetScale3D(TrailScale);

        // Hacer que la rama apunte hacia la dirección del movimiento
        if (!DeltaMovement.IsNearlyZero())
        {
            SpawnTransform.SetRotation(DeltaMovement.Rotation().Quaternion());
        }
        else
        {
            SpawnTransform.SetRotation(GetActorRotation().Quaternion());
        }

        if (InstancedTrailComponent)
        {
            InstancedTrailComponent->AddInstanceWorldSpace(SpawnTransform);
        }

        LastTrailLocation = GetActorLocation();
    }
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

    // Cambiamos o añadimos el espacio para el Checkpoint
    PlayerInputComponent->BindAction("SpawnParticles", IE_Pressed, this, &AMainPlayer::SetCheckpoint);
}

void AMainPlayer::SetCheckpoint()
{
    // Aquí podrías llamar a tu ParticulasComponent también
    if (ParticulasComponent) ParticulasComponent->SpawnParticles();

    // Lógica de Unity: El punto actual se vuelve el nuevo origen
    LastCheckpointLocation = GetActorLocation();
    MaxDistanceReached = 0.0f; // Reseteamos la "tensión" al poner la flor
    bHasCheckpoint = true;

    UE_LOG(LogTemp, Warning, TEXT("Checkpoint fijado en: %s"), *LastCheckpointLocation.ToString());
}

void AMainPlayer::MoveForward(float Value) { InputForward = Value; }
void AMainPlayer::MoveRight(float Value) { InputRight = Value; }
void AMainPlayer::ActivateParticles() { if (ParticulasComponent) ParticulasComponent->SpawnParticles(); }