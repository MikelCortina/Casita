#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraActor.h"
#include "Components/SplineComponent.h"
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
    UPROPERTY(EditAnywhere, Category = "Camera")
    ACameraActor* CameraInicial;

    UPROPERTY(EditAnywhere, Category = "Camera")
    FVector CameraLocation;

    UPROPERTY(EditAnywhere, Category = "Camera")
    FRotator CameraRotation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Spline")
    USplineComponent* CameraSpline;


    // Duración total del recorrido en segundos
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Spline")
    float TravelDuration = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Spline")
    float TargetAlpha = 1.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Spline")
    float CurrentAlpha = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Spline")
    bool bIsMoving = false;


    UFUNCTION(BlueprintCallable, Category = "Camera")
    void StartSplineMovement();

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void ActivateCamera();

private:
    void UpdateCameraOnSpline(float Alpha);
    float ElapsedTime = 0.0f;
};