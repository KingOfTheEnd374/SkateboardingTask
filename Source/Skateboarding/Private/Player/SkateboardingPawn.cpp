// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SkateboardingPawn.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ASkateboardingPawn::ASkateboardingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PhysicsSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PhysicsSphere"));
	SetRootComponent(PhysicsSphere);
	PhysicsSphere->SetVisibility(false);
	PhysicsSphere->SetGenerateOverlapEvents(false);
	PhysicsSphere->SetCollisionObjectType(ECC_PhysicsBody);
	PhysicsSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	PhysicsSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	PhysicsSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	PhysicsSphere->SetSimulatePhysics(true);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetupAttachment(PhysicsSphere);
	Root->SetUsingAbsoluteRotation(true);

	SprintArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));
	SprintArm->SetupAttachment(Root);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SprintArm);

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(Root);
	CharacterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SkateboardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkateboardMesh"));
	SkateboardMesh->SetupAttachment(Root);
	SkateboardMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ASkateboardingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkateboardingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EnforceSpeedLimit();

	RotateCharacterFromVelocity(DeltaTime);
}

// Called to bind functionality to input
void ASkateboardingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASkateboardingPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASkateboardingPawn::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASkateboardingPawn::Jump_Pressed);
}

void ASkateboardingPawn::EnforceSpeedLimit()
{
	FVector CurrentVelocity = PhysicsSphere->GetPhysicsLinearVelocity();
	FVector VelocityDir = CurrentVelocity.GetSafeNormal();
	float CurrentSpeed = CurrentVelocity.Size();

	// Don't allow speed bigger than max speed
	FVector NewVelocity = VelocityDir * fminf(CurrentSpeed, MaxSpeed);

	PhysicsSphere->SetPhysicsLinearVelocity(NewVelocity);
}

void ASkateboardingPawn::RotateCharacterFromVelocity(float DeltaTime)
{
	FRotator CurrentRot = Root->GetComponentRotation();
	FRotator TargetRot = UKismetMathLibrary::MakeRotFromX(GetHorizontalVelocity());
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, RotationSpeed);
	Root->SetWorldRotation(NewRot);
}

FVector ASkateboardingPawn::GetHorizontalVelocity()
{
	FVector Velocity = PhysicsSphere->GetPhysicsLinearVelocity();
	Velocity.Z = 0.0f;
	return Velocity;
}

void ASkateboardingPawn::MoveForward(float AxisValue)
{
	FVector ForwardDir = Root->GetForwardVector();
	PhysicsSphere->AddForce(ForwardDir * 1000.0f * AxisValue, NAME_None, true);
}

void ASkateboardingPawn::MoveRight(float AxisValue)
{
	FVector RightDir = Root->GetRightVector();
	float TurningMultiplier = fmaxf(GetHorizontalVelocity().Size() / 500.0f, 1.0f);
	PhysicsSphere->AddForce(RightDir * 1000.0f * AxisValue * TurningMultiplier, NAME_None, true);
}

void ASkateboardingPawn::Jump_Pressed()
{
	// TODO: Disable jumping while in air.
	PhysicsSphere->AddImpulse(FVector(0.0f, 0.0f, 1000.0f), NAME_None, true);
}