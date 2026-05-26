#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraActor.h"
#include "Camara/CameraManager.h"
#include "Components/PortalComponent.h"
#include "Components/AbsorptionComponent.h"
#include "NextLevel.generated.h"

class UBoxComponent;

UCLASS()
class CASITA_API ANextLevel : public AActor
{
    GENERATED_BODY()

public:
    ANextLevel();

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* TriggerBox;

    UPROPERTY(EditAnywhere, Category = "Portal")
    ANextLevel* LinkedPortal;

    UPROPERTY(EditAnywhere, Category = "Camera|Estatica")
    ACameraActor* TargetCamera;

    UPROPERTY(EditAnywhere, Category = "Camera|Estatica")
    float BlendTime = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Camera|Spline")
    ACameraManager* CameraManagerRef;

    UPROPERTY(EditAnywhere, Category = "Camera|Spline")
    bool bUseSplineCamera = true;

    UPROPERTY(EditAnywhere, Category = "Trigger")
    bool bTriggerOnce = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
    UPortalComponent* PortalComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
    UAbsorptionComponent* AbsorptionComponent;

protected:
    virtual void BeginPlay() override;

    bool bAlreadyTriggered = false;

    void DisableTrigger();
    void CleanupAndDestroy();

    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32                OtherBodyIndex,
        bool                 bFromSweep,
        const FHitResult& SweepResult);

private:
    void CheckCameraMovement();

    FTimerHandle CameraCheckTimer;
};