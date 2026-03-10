// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraActor.h"
#include "CameraManager.generated.h"

UCLASS()
class CASITA_API ACameraManager : public AActor
{
	GENERATED_BODY()

public:
	ACameraManager();

protected:
	virtual void BeginPlay() override;

public:
	// Cámara que veremos al inicio
	UPROPERTY(EditAnywhere, Category = "Camera")
	ACameraActor* CameraInicial;

	// Posición y rotación de la cámara inicial
	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector CameraLocation;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FRotator CameraRotation;
};