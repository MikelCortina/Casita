#include "House/House.h"

// Sets default values
AHouse::AHouse()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AHouse::BeginPlay()
{
	Super::BeginPlay();
}