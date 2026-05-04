// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Valve.generated.h"

UCLASS()
class CASITA_API AValve : public AActor
{
    GENERATED_BODY()

public:
    AValve();
    void TryInteract();

protected:
    virtual void BeginPlay() override;

    // Referencia al agua que controla esta llave — asígnala en el editor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valve")
    TObjectPtr<class AHardWaterStream> LinkedWaterStream;

    // Tecla de interacción — configúrala en Project Settings -> Input
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valve")
    FName InteractInputAction = TEXT("Interact");

private:
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    class USphereComponent* ProximityTrigger;

    bool bPlayerInRange = false;
    bool bActivated = false;

    UFUNCTION()
    void OnProximityBeginOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnProximityEndOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);


};
