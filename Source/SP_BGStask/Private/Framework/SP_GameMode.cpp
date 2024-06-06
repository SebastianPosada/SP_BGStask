// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SP_GameMode.h"
#include "Kismet/GameplayStatics.h"

void ASP_GameMode::PauseGame(bool SetPauseGame)
{
	// GameplayStats to pause the current session
	UGameplayStatics::SetGamePaused(GetWorld(), SetPauseGame);
}
