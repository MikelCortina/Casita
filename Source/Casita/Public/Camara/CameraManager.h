#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Camera/CameraComponent.h"
#include "CameraManager.generated.h"

UCLASS()
class CASITA_API ACameraManager : public AActor
{
    GENERATED_BODY()

public:
    ACameraManager();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
    bool bAllowSplineRotation = true;

    void MoveToNextPoint();
    void ActivateCamera();
    void DisableInput();
    void EnableInput();
    void MoveToFirstPoint();
    void StartSplineIntro();

    // Intro: movimiento desde punto fijo al spline
    bool bIntroMovement = false;
    FVector IntroStartLocation;
    FRotator IntroStartRotation;
    FVector IntroTargetLocation;
    FRotator IntroTargetRotation;

    // Intro: vuelta completa al spline
    UPROPERTY(EditAnywhere, Category = "Intro")
    float SplineIntroDuration = 20.f;

    bool IsCameraMoving() const { return bIsMoving || bSplineIntro; }

private:
    void UpdateCameraOnSpline(float Alpha);

public:
    UPROPERTY(EditAnywhere)
    USplineComponent* CameraSpline;

    UPROPERTY(EditAnywhere)
    UCameraComponent* CameraComp;

    UPROPERTY(EditAnywhere)
    float TravelDuration = 2.0f;

private:
    int32 CurrentPointIndex = 0;

    float SegmentAlphaStart = 0.f;
    float SegmentAlphaEnd = 0.f;
    float ElapsedTime = 0.f;

    bool bIsMoving = false;
    bool bSplineIntro = false;
    float SplineIntroElapsed = 0.f;
};