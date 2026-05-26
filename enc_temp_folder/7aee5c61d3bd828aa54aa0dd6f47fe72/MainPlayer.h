#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UI/InGameUI.h"
#include "MainPlayer.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPlayer, Log, All);

class UBoxComponent;
class UStaticMeshComponent;
class UParticulasComponent;
class UInstancedStaticMeshComponent;
class AValve;

UCLASS()
class CASITA_API AMainPlayer : public APawn
{
    GENERATED_BODY()

public:
    AMainPlayer();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* CollisionBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticulasComponent* ParticulasComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Branch Trail")
    UInstancedStaticMeshComponent* InstancedTrailComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch Trail")
    float SpawnTrailDistance = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch Trail")
    FVector TrailScale = FVector(0.5f, 0.5f, 0.5f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float GravityFactor = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    int32 MaxCheckpointUses = 10;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
    int32 RemainingCheckpoints = 10;

    UPROPERTY()
    AValve* NearbyValve;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> PauseMenuClass;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> InGameUIClass;

    UFUNCTION(BlueprintImplementableEvent)
    void UpdateUI();

    void SetCameraIsMoving(bool bNewValue)
    {
        bCameraMoving = bNewValue;
        SetActorHiddenInGame(bNewValue);
    }

    void SetNearbyValve(AValve* Valve) { NearbyValve = Valve; }
    void ClearNearbyValve() { NearbyValve = nullptr; }

    void ResetPlayerState();
    void SetCheckpoint();
    void ResetGravity();
    void TryInteractWithValve();

protected:
    virtual void BeginPlay() override;

private:
    UUserWidget* PauseMenuInstance = nullptr;
    UInGameUI* InGameUIInstance = nullptr;

    bool  bIsPaused = false;
    bool  bCameraMoving = false;
    bool  bHasCheckpoint = false;

    float InputForward = 0.0f;
    float InputRight = 0.0f;
    float MaxDistanceReached = 0.0f;

    FVector LastCheckpointLocation;
    FVector LastTrailLocation;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void ActivateParticles();
    void TogglePause();
};