// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraActor.h"
#include "Camara/CameraManager.h"
#include "NextLevel.generated.h"

class UBoxComponent;

UCLASS()
class CASITA_API ANextLevel : public AActor
{
    GENERATED_BODY()

public:
    ANextLevel();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* TriggerBox;

    UPROPERTY(EditAnywhere, Category = "Camera|Estatica")
    ACameraActor* TargetCamera;

    UPROPERTY(EditAnywhere, Category = "Camera|Estatica")
    float BlendTime = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Camera|Spline")
    ACameraManager* CameraManagerRef;

    UPROPERTY(EditAnywhere, Category = "Camera|Spline")
    bool bUseSplineCamera = true;

    UPROPERTY(EditAnywhere, Category = "Player")
    FVector PlayerNewLocation;

    UPROPERTY(EditAnywhere, Category = "Player")
    bool bTeleportPlayer = true;

    UPROPERTY(EditAnywhere, Category = "Trigger")
    bool bTriggerOnce = true;

protected:
    bool bAlreadyTriggered = false;

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent,
        class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
};