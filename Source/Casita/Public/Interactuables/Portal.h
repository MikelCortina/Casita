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

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal")
    UBoxComponent* PortalBoxA;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal")
    UBoxComponent* PortalBoxB;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* SceneComponent;

    // ----- NUEVO -----
    UFUNCTION()
    void OnPortalOverlapA(
        UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnPortalOverlapB(
        UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    // Para evitar rebote inmediato
    TMap<TWeakObjectPtr<AActor>, float> Cooldowns;
    float CooldownTime = 0.15f; // segundos

    bool CanTeleport(AActor* Actor) const;
    void PutInCooldown(AActor* Actor);

public:
    virtual void Tick(float DeltaTime) override;
};