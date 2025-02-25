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
	float MaxSpeed = 2000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RotationSpeed = 20.0f;

	UPROPERTY(BlueprintReadOnly)
	bool Boosting;

	UPROPERTY(BlueprintReadOnly)
	bool Jumping;

	UPROPERTY(BlueprintReadOnly)
	bool Falling;

	UPROPERTY(BlueprintReadOnly)
	float TurnTilt;

	UFUNCTION(BlueprintCallable)
	FVector GetHorizontalVelocity();

	UFUNCTION(BlueprintCallable)
	bool OnGround();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	float DeltaT;

	UFUNCTION()
	void EnforceSpeedLimit();

	UFUNCTION()
	void RotateCharacterFromVelocity(float DeltaTime);

	UFUNCTION()
	void AlignSkateboardToGround(float DeltaTime);

	UFUNCTION()
	FVector SkateboardAlignTrace(FVector TraceStart);

	UPROPERTY()
	float LandedCheckInterval = 0.05f;

	UPROPERTY()
	float TimeUntilLandedCheck = 0.0f;

	UFUNCTION()
	void CheckLanded(float DeltaTime);

	UFUNCTION()
	void Landed();

	// INPUTS
	UFUNCTION()
	void MoveForward(float AxisValue);
	UFUNCTION()
	void MoveRight(float AxisValue);
	UFUNCTION()
	void Jump_Pressed();
	UFUNCTION()
	void QuitGame();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
