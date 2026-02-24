// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CASITA_API GameManager
{
public:
	GameManager();
	~GameManager();


	int Flores;

	UFUNCTION(BlueprintCallable, Category = "GameManager")
	void CrearFlor();
};
