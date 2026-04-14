#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPlayer.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UParticulasComponent;
// --- NUEVO: Declaración del componente instanciado ---
class UInstancedStaticMeshComponent;

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* CollisionBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticulasComponent* ParticulasComponent;

    // --- NUEVO: Componente para las copias de la rama ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Branch Trail")
    UInstancedStaticMeshComponent* InstancedTrailComponent;

    // --- NUEVO: Distancia requerida para soltar una nueva copia ---
  // --- NUEVO: Distancia requerida para soltar una nueva copia ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch Trail")
    float SpawnTrailDistance = 50.0f;

    // --- NUEVO: Escala de las copias del trail ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch Trail")
    FVector TrailScale = FVector(0.5f, 0.5f, 0.5f); // 0.5 significa la mitad de su tamaño

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float GravityFactor = 0.5f;

    void SetCameraIsMoving(bool bNewValue) {
        bCameraMoving = bNewValue;
        SetActorHiddenInGame(bNewValue);
    }
    // --- NUEVO: Limitar los checkpoints ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    int32 MaxCheckpointUses = 5;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
    int32 RemainingCheckpoints = 5;

    // Función para resetear todo al cruzar el portal
    void ResetPlayerState();
    void SetCheckpoint();

private:
    float InputForward = 0.0f;
    float InputRight = 0.0f;

    FVector LastCheckpointLocation;
    float MaxDistanceReached;
    bool bHasCheckpoint = false;
    bool bCameraMoving = false;
    // --- NUEVO: Para saber dónde soltamos la última rama ---
    FVector LastTrailLocation;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void ActivateParticles();

};