// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include "SkateboardingPawn.generated.h"

UCLASS()
class SKATEBOARDING_API ASkateboardingPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASkateboardingPawn();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* PhysicsSphere;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* Root;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USpringArmComponent* SprintArm;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* SkateboardMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxSpeed = 1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RotationSpeed = 20.0f;

	UFUNCTION(BlueprintCallable)
	FVector GetHorizontalVelocity();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void EnforceSpeedLimit();

	UFUNCTION()
	void RotateCharacterFromVelocity(float DeltaTime);
	

	// INPUTS
	UFUNCTION()
	void MoveForward(float AxisValue);
	UFUNCTION()
	void MoveRight(float AxisValue);
	UFUNCTION()
	void Jump_Pressed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
