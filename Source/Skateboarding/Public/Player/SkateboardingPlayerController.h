// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SkateboardingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SKATEBOARDING_API ASkateboardingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Score = 0;
	
};
