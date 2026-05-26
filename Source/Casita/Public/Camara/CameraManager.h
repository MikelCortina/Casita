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

    UPROPERTY(EditAnywhere)
    USplineComponent* CameraSpline;

    UPROPERTY(EditAnywhere)
    UCameraComponent* CameraComp;

    UPROPERTY(EditAnywhere)
    float TravelDuration = 2.0f;

    UPROPERTY(EditAnywhere, Category = "Intro")
    float SplineIntroDuration = 20.f;

    bool bAllowSplineRotation = true;
    bool bIntroMovement = false;

    FVector  IntroStartLocation;
    FRotator IntroStartRotation;
    FVector  IntroTargetLocation;
    FRotator IntroTargetRotation;

    void MoveToNextPoint();
    void MoveToFirstPoint();
    void StartSplineIntro();
    void ActivateCamera();
    void DisableInput();
    void EnableInput();

    bool IsCameraMoving() const { return bIsMoving || bSplineIntro; }

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void OnConstruction(const FTransform& Transform) override;

private:
    void UpdateCameraOnSpline(float Alpha);

    int32 CurrentPointIndex = 0;
    float SegmentAlphaStart = 0.f;
    float SegmentAlphaEnd = 0.f;
    float ElapsedTime = 0.f;
    float SplineIntroElapsed = 0.f;

    bool bIsMoving = false;
    bool bSplineIntro = false;
};