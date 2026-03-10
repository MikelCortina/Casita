// Fill out your copyright notice in the Description page of Project Settings.


#include "Camara/CameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

ACameraManager::ACameraManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACameraManager::BeginPlay()
{
	Super::BeginPlay();

	if (!CameraInicial)
	{
		// Si no hay cámara asignada, creamos una nueva
		CameraInicial = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation);
	}
	else
	{
		// Si ya está asignada, movemos/rotamos la cámara al lugar correcto
		CameraInicial->SetActorLocation(CameraLocation);
		CameraInicial->SetActorRotation(CameraRotation);
	}

	// Cambiar el view target del PlayerController a esta cámara
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && CameraInicial)
	{
		PC->SetViewTargetWithBlend(CameraInicial, 0.f); // 0 segundos = instantáneo
	}
}
