// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "Obstacle.generated.h"

UCLASS()
class SKATEBOARDING_API AObstacle : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AObstacle();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UBoxComponent* JumpingOverCollision;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
