// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraActor.h"
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
	// Trigger que detecta al jugador
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerBox;

	// Cámara a la que rotará cuando el jugador llegue
	UPROPERTY(EditAnywhere, Category = "Camera")
	ACameraActor* TargetCamera;

	// Duración del blend de la cámara
	UPROPERTY(EditAnywhere, Category = "Camera")
	float BlendTime = 1.0f;

	// Posición donde queremos teletransportar al jugador (opcional)
	UPROPERTY(EditAnywhere, Category = "Player")
	FVector PlayerNewLocation;

	// Función que se llama al entrar en el trigger
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};