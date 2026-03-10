// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPlayer.generated.h"

class UParticulasComponent;

UCLASS()
class CASITA_API AMainPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Aquí puedes agregar componentes, variables y funciones privadas para tu jugador
	UPROPERTY(VisibleAnywhere)
	UParticulasComponent* ParticulasComponent;

	void ActivateParticles();
};
