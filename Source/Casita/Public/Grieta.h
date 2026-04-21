#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/FissureComponent.h"
#include "Grieta.generated.h"

UCLASS()
class CASITA_API AGrieta : public AActor
{
    GENERATED_BODY()

public:
    AGrieta();

protected:
    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* CollisionBox;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UFissureComponent> FissureComponent;

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* Mesh;

    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    bool bActivated = false;
};