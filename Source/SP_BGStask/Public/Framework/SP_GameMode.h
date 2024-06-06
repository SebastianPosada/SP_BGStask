// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SP_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class SP_BGSTASK_API ASP_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "A_GameplayFunctionalities")
	void PauseGame(bool SetPauseGame);
};
