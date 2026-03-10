// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainPlayer.h"
#include "Components/ParticulasComponent.h"
#include "Components/InputComponent.h"


// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticulasComponent = CreateDefaultSubobject<UParticulasComponent>(TEXT("ParticulasComponent"));
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("SpawnParticles", IE_Pressed, this, &AMainPlayer::ActivateParticles);
}

void AMainPlayer::ActivateParticles()
{
	if (ParticulasComponent)
	{
		ParticulasComponent->SpawnParticles();
	}
}
