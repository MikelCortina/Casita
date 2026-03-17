#include "Camara/CameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

ACameraManager::ACameraManager()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraSpline = CreateDefaultSubobject<USplineComponent>(TEXT("CameraSpline"));
    RootComponent = CameraSpline;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(RootComponent);
    CameraComp->bAutoActivate = false;
}

void ACameraManager::BeginPlay()
{
    Super::BeginPlay();

    if (!CameraSpline || CameraSpline->GetNumberOfSplinePoints() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("CameraManager [%s]: spline vacío"), *GetName());
        return;
    }

    CameraComp->Deactivate();

    // Colocar la cámara en el punto 0 sin mover el actor raíz
    CurrentPointIndex = 0;
    UpdateCameraOnSpline(0.0f);

    UE_LOG(LogTemp, Warning, TEXT("CameraManager: %d puntos en el spline"),
        CameraSpline->GetNumberOfSplinePoints());
}

void ACameraManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsMoving) return;

    ElapsedTime += DeltaTime;
    float T = FMath::Clamp(ElapsedTime / TravelDuration, 0.0f, 1.0f);

    float Alpha = FMath::Lerp(SegmentAlphaStart, SegmentAlphaEnd, T);
    UpdateCameraOnSpline(Alpha);

    if (T >= 1.0f)
    {
        bIsMoving = false;
        ElapsedTime = 0.0f;
        UpdateCameraOnSpline(SegmentAlphaEnd);
        UE_LOG(LogTemp, Log, TEXT("CameraManager: llegó al punto %d"), CurrentPointIndex);
    }
}

void ACameraManager::UpdateCameraOnSpline(float Alpha)
{
    if (!CameraSpline || !CameraComp) return;

    float Distance = Alpha * CameraSpline->GetSplineLength();

    // Posición y rotación en coordenadas del spline (locales al actor)
    FVector  NewPos = CameraSpline->GetLocationAtDistanceAlongSpline(
        Distance, ESplineCoordinateSpace::Local);
    FRotator NewRot = CameraSpline->GetRotationAtDistanceAlongSpline(
        Distance, ESplineCoordinateSpace::Local);

    // Mover solo el componente cámara, NO el actor raíz
    CameraComp->SetRelativeLocation(NewPos);
    CameraComp->SetRelativeRotation(NewRot + FRotator(0, -90, 0));
}

void ACameraManager::MoveToNextPoint()
{
    if (bIsMoving) return;

    if (!CameraSpline)
    {
        UE_LOG(LogTemp, Error, TEXT("CameraManager: CameraSpline es null"));
        return;
    }

    int32 TotalPoints = CameraSpline->GetNumberOfSplinePoints();
    if (CurrentPointIndex >= TotalPoints - 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("CameraManager: último punto alcanzado (%d/%d)"),
            CurrentPointIndex, TotalPoints - 1);
        return;
    }

    float SplineLength = CameraSpline->GetSplineLength();
    float DistStart = CameraSpline->GetDistanceAlongSplineAtSplinePoint(CurrentPointIndex);
    float DistEnd = CameraSpline->GetDistanceAlongSplineAtSplinePoint(CurrentPointIndex + 1);

    SegmentAlphaStart = DistStart / SplineLength;
    SegmentAlphaEnd = DistEnd / SplineLength;

    UE_LOG(LogTemp, Warning, TEXT("CameraManager: moviendo del punto %d al %d (alpha %.2f → %.2f)"),
        CurrentPointIndex, CurrentPointIndex + 1, SegmentAlphaStart, SegmentAlphaEnd);

    CurrentPointIndex++;
    ElapsedTime = 0.0f;
    bIsMoving = true;

    ActivateCamera();
}

void ACameraManager::ActivateCamera()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    CameraComp->Activate();
    PC->SetViewTargetWithBlend(this, 0.5f);
}