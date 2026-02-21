// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}
void GameManager::CrearFlor()
{
	if (Flores > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creando Flor..."));
		Flores--;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No hay flores disponibles"));
	}
}