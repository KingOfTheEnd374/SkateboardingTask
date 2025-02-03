// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SkateboardingPawn.h"

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

}

// Called to bind functionality to input
void ASkateboardingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASkateboardingPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASkateboardingPawn::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASkateboardingPawn::Jump_Pressed);
}

void ASkateboardingPawn::MoveForward(float AxisValue)
{
}

void ASkateboardingPawn::MoveRight(float AxisValue)
{
}

void ASkateboardingPawn::Jump_Pressed()
{
}