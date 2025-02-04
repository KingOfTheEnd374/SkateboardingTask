// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateboardingGameMode.h"
#include "Player/SkateboardingPawn.h"
#include "Player/SkateboardingPlayerController.h"

ASkateboardingGameMode::ASkateboardingGameMode()
{
	DefaultPawnClass = ASkateboardingPawn::StaticClass();
	PlayerControllerClass = ASkateboardingPlayerController::StaticClass();
}