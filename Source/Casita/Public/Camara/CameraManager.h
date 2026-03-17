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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USplineComponent* CameraSpline;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float TravelDuration = 5.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    bool bIsMoving = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    int32 CurrentPointIndex = 0;

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void MoveToNextPoint();

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void ActivateCamera();

private:
    void UpdateCameraOnSpline(float Alpha);

    float SegmentAlphaStart = 0.0f;
    float SegmentAlphaEnd = 0.0f;
    float ElapsedTime = 0.0f;
};