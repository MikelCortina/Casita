#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPlayer.generated.h"

class UParticulasComponent;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class CASITA_API AMainPlayer : public APawn
{
    GENERATED_BODY()

public:
    AMainPlayer();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Collision")
    UBoxComponent* CollisionBox;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UParticulasComponent* ParticulasComponent;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 600.0f;

    float InputForward = 0.0f;
    float InputRight = 0.0f;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void ActivateParticles();

    // --- NUEVAS VARIABLES PARA EL SISTEMA DE GRAVEDAD ---
    UPROPERTY(EditAnywhere, Category = "Movement|Gravity")
    float GravityFactor = 2.0f; // Multiplicador de caída

    FVector LastCheckpointLocation;
    bool bHasCheckpoint = true;
    float MaxDistanceReached = 0.0f; // Para que no se reduzca al volver

    FVector CurrentGravityVelocity; // Acumulador de caída

    void SetCheckpoint();
    void ApplyDistanceGravity(float DeltaTime);


};