// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera.generated.h"

class AMainCharacter;

UCLASS()
class DEMAESCAPE_API ACamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Set forward vector to face main character
	virtual void FaceMainCharacter();

	AMainCharacter* MainCharacter;
	FVector mainCharacterPos;
	FVector cameraPos;
};
