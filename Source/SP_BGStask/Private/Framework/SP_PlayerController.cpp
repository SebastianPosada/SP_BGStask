// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SP_PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Framework/SP_GameMode.h"


void ASP_PlayerController::TogglePause()
{
	ASP_GameMode* GameModeRef = Cast<ASP_GameMode>(GetWorld() -> GetAuthGameMode());
	
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		GameModeRef -> PauseGame(false);
		
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		return;
	}
	
	{
		PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
		if (PauseMenuInstance)
		{
			// Gets the gamemode ref and pauses the game
			
			GameModeRef -> PauseGame(true);
		
			PauseMenuInstance -> AddToViewport(100);
			
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}
