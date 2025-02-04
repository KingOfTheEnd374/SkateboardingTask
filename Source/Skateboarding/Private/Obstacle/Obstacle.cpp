// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle/Obstacle.h"
#include <Player/SkateboardingPawn.h>
#include <Player/SkateboardingPlayerController.h>

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));
	Mesh->SetGenerateOverlapEvents(true);

	JumpingOverCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("JumpingOverCollision"));
	JumpingOverCollision->SetupAttachment(Mesh);
	JumpingOverCollision->SetGenerateOverlapEvents(true);
	JumpingOverCollision->SetCollisionObjectType(ECC_WorldDynamic);
	JumpingOverCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	JumpingOverCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
	JumpingOverCollision->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::BeginOverlap);
}

void AObstacle::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASkateboardingPawn* Player = Cast<ASkateboardingPawn>(OtherActor);
	if (Player)
	{
		Cast<ASkateboardingPlayerController>(Player->GetController())->Score++;
	}
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

