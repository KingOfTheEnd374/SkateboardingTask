// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SkateboardingPawn.h"
#include <Kismet/KismetMathLibrary.h>
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASkateboardingPawn::ASkateboardingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PhysicsSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PhysicsSphere"));
	SetRootComponent(PhysicsSphere);
	PhysicsSphere->SetVisibility(false);
	PhysicsSphere->SetGenerateOverlapEvents(true);
	PhysicsSphere->SetCollisionObjectType(ECC_Pawn);
	PhysicsSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	PhysicsSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	PhysicsSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
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

	DeltaT = DeltaTime;

	EnforceSpeedLimit();

	RotateCharacterFromVelocity(DeltaTime);

	CheckLanded(DeltaTime);

	PhysicsSphere->SetPhysicsAngularVelocityInDegrees(FVector(0.0f));
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
	FVector CurrentVelocity = GetHorizontalVelocity();
	if (CurrentVelocity.Size() < 1.0f) return;
	FRotator CurrentRot = Root->GetComponentRotation();
	FRotator TargetRot = UKismetMathLibrary::MakeRotFromX(CurrentVelocity);
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, RotationSpeed);
	Root->SetWorldRotation(NewRot);
}

void ASkateboardingPawn::CheckLanded(float DeltaTime)
{
	TimeUntilLandedCheck -= DeltaTime;
	if (TimeUntilLandedCheck <= 0.0f)
	{
		bool OnGroundNow = OnGround();
		if (Falling && OnGroundNow)
		{
			Landed();
		}
		TimeUntilLandedCheck = LandedCheckInterval;
		Falling = !OnGroundNow;
	}
}

void ASkateboardingPawn::Landed()
{
	Jumping = false;
}

FVector ASkateboardingPawn::GetHorizontalVelocity()
{
	FVector Velocity = PhysicsSphere->GetPhysicsLinearVelocity();
	Velocity.Z = 0.0f;
	return Velocity;
}

bool ASkateboardingPawn::OnGround()
{
	// Sphere trace downwards, if hits then pawn is on ground.
	FHitResult Hit;
	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), 
		GetActorLocation(), 
		GetActorLocation() - FVector(0.0f, 0.0f, 5.0f), 49.0f, 
		{ UEngineTypes::ConvertToObjectType(ECC_WorldStatic) }, 
		false, {}, EDrawDebugTrace::None, Hit, true);

	return Hit.bBlockingHit;
}

void ASkateboardingPawn::MoveForward(float AxisValue)
{
	FVector ForwardDir = Root->GetForwardVector();
	float AccelerationMultiplier = 1.0f;
	if (Falling) AccelerationMultiplier *= 0.2f;

	// To avoid turning in place
	if (PhysicsSphere->GetPhysicsLinearVelocity().Size() < 100.0f) AxisValue = fmaxf(AxisValue, 0.0f);
	PhysicsSphere->AddForce(ForwardDir * 1000.0f * AxisValue * AccelerationMultiplier, NAME_None, true);	

	Boosting = AxisValue > 0.0f;
}

void ASkateboardingPawn::MoveRight(float AxisValue)
{

	FVector RightDir = Root->GetRightVector();
	float TurningMultiplier = fmaxf(GetHorizontalVelocity().Size() / 500.0f, 1.0f);
	if (Falling) TurningMultiplier *= 0.2f;

	// To avoid turning in place
	if (PhysicsSphere->GetPhysicsLinearVelocity().Size() < 100.0f) return;
	PhysicsSphere->AddForce(RightDir * 1000.0f * AxisValue * TurningMultiplier, NAME_None, true);

	TurnTilt = FMath::FInterpTo(TurnTilt, AxisValue, DeltaT, 2.0f);
}

void ASkateboardingPawn::Jump_Pressed()
{
	if (OnGround())
	{
		PhysicsSphere->AddImpulse(FVector(0.0f, 0.0f, 500.0f), NAME_None, true);
		Jumping = true;
	}
}