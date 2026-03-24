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

public:
    void MoveToNextPoint();
    void ActivateCamera();

    void DisableInput();
    void EnableInput();

    bool IsCameraMoving() const { return bIsMoving; }

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

    float SegmentAlphaStart;
    float SegmentAlphaEnd;
    float ElapsedTime = 0.0f;

    bool bIsMoving = false;
};