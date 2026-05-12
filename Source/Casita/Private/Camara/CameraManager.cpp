#include "Camara/CameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Player/MainPlayer.h"

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
        return;

    CameraComp->Deactivate();

    MoveToFirstPoint();

    CurrentPointIndex = 0;
}

void ACameraManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIntroMovement)
    {
        ElapsedTime += DeltaTime;

        float T = FMath::Clamp(
            ElapsedTime / TravelDuration,
            0.f,
            1.f);

        FVector NewLocation = FMath::Lerp(
            IntroStartLocation,
            IntroTargetLocation,
            T);

        FRotator NewRotation = FMath::Lerp(
            IntroStartRotation,
            IntroTargetRotation,
            T);

        SetActorLocation(NewLocation);
        SetActorRotation(NewRotation);

        if (T >= 1.f)
        {
            bIntroMovement = false;
            ElapsedTime = 0.f;

            EnableInput();
        }

        return;
    }


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
        EnableInput();
    }
}

void ACameraManager::UpdateCameraOnSpline(float Alpha)
{
    if (!CameraSpline || !CameraComp) return;

    float Distance = Alpha * CameraSpline->GetSplineLength();

    FVector NewPos = CameraSpline->GetLocationAtDistanceAlongSpline(
        Distance, ESplineCoordinateSpace::Local);

    FRotator NewRot = CameraSpline->GetRotationAtDistanceAlongSpline(
        Distance, ESplineCoordinateSpace::Local);

    CameraComp->SetRelativeLocation(NewPos);
    CameraComp->SetRelativeRotation(NewRot + FRotator(0, 0, 0));
}

void ACameraManager::MoveToNextPoint()
{
    if (bIsMoving) return;

    int32 TotalPoints = CameraSpline->GetNumberOfSplinePoints();
    if (CurrentPointIndex >= TotalPoints - 1) return;

    float SplineLength = CameraSpline->GetSplineLength();

    float DistStart = CameraSpline->GetDistanceAlongSplineAtSplinePoint(CurrentPointIndex);
    float DistEnd = CameraSpline->GetDistanceAlongSplineAtSplinePoint(CurrentPointIndex + 1);

    SegmentAlphaStart = DistStart / SplineLength;
    SegmentAlphaEnd = DistEnd / SplineLength;

    CurrentPointIndex++;
    ElapsedTime = 0.0f;
    bIsMoving = true;

    ActivateCamera();
    DisableInput();
}

void ACameraManager::ActivateCamera()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    CameraComp->Activate();
    PC->SetViewTargetWithBlend(this, 0.5f);
}

void ACameraManager::DisableInput()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    PC->SetIgnoreMoveInput(true);
    PC->SetIgnoreLookInput(true);
    AMainPlayer* MainPlayer = Cast<AMainPlayer>(PC->GetPawn());
    if (IsValid(MainPlayer))
    {
        MainPlayer->SetCameraIsMoving(true);
    }
}

void ACameraManager::EnableInput()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    PC->SetIgnoreMoveInput(false);
    PC->SetIgnoreLookInput(false);
    AMainPlayer* MainPlayer = Cast<AMainPlayer>(PC->GetPawn());
    if (IsValid(MainPlayer))
    {
        MainPlayer->SetCameraIsMoving(false);
    }
}
void ACameraManager::MoveToFirstPoint()
{
    if (!CameraSpline || bIsMoving)
        return;

    // Posición inicial fija
    FVector StartLocation = FVector(1310.f, 3710.f, 830.f);
    FRotator StartRotation = FRotator(0.f, 0.f, 0.f);

    // Primera posición del spline
    FVector FirstSplineLocation =
        CameraSpline->GetLocationAtSplinePoint(
            0,
            ESplineCoordinateSpace::World);

    FRotator FirstSplineRotation =
        CameraSpline->GetRotationAtSplinePoint(
            0,
            ESplineCoordinateSpace::World);

    // Colocar cámara al inicio
    SetActorRotation(FirstSplineRotation + FRotator(0, 0, 0));
    SetActorRotation(StartRotation);

    // Guardar valores para interpolación
    IntroStartLocation = StartLocation;
    IntroStartRotation = StartRotation;

    IntroTargetLocation = FirstSplineLocation;
    IntroTargetRotation = FirstSplineRotation;

    // Empezar movimiento
    ElapsedTime = 0.f;
    bIntroMovement = true;

    ActivateCamera();
    DisableInput();
}
