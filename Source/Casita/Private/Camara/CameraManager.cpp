#include "Camara/CameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

ACameraManager::ACameraManager()
{
    PrimaryActorTick.bCanEverTick = true; // Necesario para el movimiento

    // Crear el Spline como componente raíz
    CameraSpline = CreateDefaultSubobject<USplineComponent>(TEXT("CameraSpline"));
    RootComponent = CameraSpline;
}

void ACameraManager::BeginPlay()
{
    Super::BeginPlay();

    if (!CameraInicial)
    {
        CameraInicial = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation);
    }
    else
    {
        CameraInicial->SetActorLocation(CameraLocation);
        CameraInicial->SetActorRotation(CameraRotation);
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC && CameraInicial)
    {
        PC->SetViewTargetWithBlend(CameraInicial, 0.f);
    }

    UpdateCameraOnSpline(0.0f);
}

void ACameraManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsMoving) return;

    ElapsedTime += DeltaTime;
    CurrentAlpha = FMath::Clamp(ElapsedTime / TravelDuration, 0.0f, 1.0f);

    UpdateCameraOnSpline(CurrentAlpha);

    // Llegamos al final
    if (CurrentAlpha >= TargetAlpha)
    {
        CurrentAlpha = TargetAlpha;
        bIsMoving = false;
        ElapsedTime = 0.0f;
    }
}

void ACameraManager::UpdateCameraOnSpline(float Alpha)
{
    if (!CameraSpline || !CameraInicial) return;

    float Distance = Alpha * CameraSpline->GetSplineLength();

    FVector  NewPos = CameraSpline->GetLocationAtDistanceAlongSpline(
                          Distance, ESplineCoordinateSpace::World);
    FRotator NewRot = CameraSpline->GetRotationAtDistanceAlongSpline(
                          Distance, ESplineCoordinateSpace::World);

    CameraInicial->SetActorLocation(NewPos);
    CameraInicial->SetActorRotation(NewRot);
}

void ACameraManager::StartSplineMovement()
{
    if (bIsMoving) return; // Evitar relanzar si ya se mueve

    ElapsedTime = 0.0f;
    CurrentAlpha = 0.0f;
    bIsMoving = true;

    // Activar la cámara al iniciar el movimiento
    ActivateCamera();
}

void ACameraManager::ActivateCamera()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC && CameraInicial)
    {
        PC->SetViewTargetWithBlend(CameraInicial, 0.5f);
    }
}
