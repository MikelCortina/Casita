#include "Player/MainPlayer.h"
#include "Components/ParticulasComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
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

    ParticulasComponent = CreateDefaultSubobject<UParticulasComponent>(TEXT("ParticulasComponent"));
}

void AMainPlayer::BeginPlay()
{
    Super::BeginPlay();
}

void AMainPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (InputForward != 0.0f || InputRight != 0.0f)
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

                // Calculamos el desplazamiento de este frame
                FVector DeltaMovement = MovementDirection * MoveSpeed * DeltaTime;

                // Usamos AddActorWorldOffset con bSweep = true
                // Esto permite que el motor maneje la colisión de forma nativa
                FHitResult Hit;
                AddActorWorldOffset(DeltaMovement, true, &Hit);

                // Si chocamos, intentamos movernos solo en la dirección de la normal 
                // para permitir un deslizamiento básico manual
                if (Hit.IsValidBlockingHit())
                {
                    FVector SlideVector = FVector::VectorPlaneProject(DeltaMovement, Hit.Normal);
                    AddActorWorldOffset(SlideVector, true);
                }
            }
        }
    }
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);
    PlayerInputComponent->BindAction("SpawnParticles", IE_Pressed, this, &AMainPlayer::ActivateParticles);
}

void AMainPlayer::MoveForward(float Value) { InputForward = Value; }
void AMainPlayer::MoveRight(float Value) { InputRight = Value; }
void AMainPlayer::ActivateParticles() { if (ParticulasComponent) ParticulasComponent->SpawnParticles(); }