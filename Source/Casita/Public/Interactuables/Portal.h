#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

class UBoxComponent;

UCLASS()
class CASITA_API APortal : public AActor
{
    GENERATED_BODY()

public:
    APortal();

protected:
    virtual void BeginPlay() override;

    /** Root */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
    USceneComponent* SceneComponent;

    /** Trigger de entrada A */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal")
    UBoxComponent* PortalBoxA;

    /** Trigger de entrada B */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal")
    UBoxComponent* PortalBoxB;

    /** Handler overlap para A */
    UFUNCTION()
    void OnPortalOverlapA(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    /** Handler overlap para B */
    UFUNCTION()
    void OnPortalOverlapB(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

public:
    virtual void Tick(float DeltaTime) override;

    /** Tiempo mínimo para no hacer “ping‑pong” */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
    float CooldownTime = 5.0f;

    FTimerHandle TimerHandle;
    void ReEnablePortal();

};