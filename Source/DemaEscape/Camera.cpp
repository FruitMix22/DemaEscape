// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera.h"
#include "Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACamera::ACamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACamera::BeginPlay()
{
	Super::BeginPlay();

	// Get main character reference
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if(!MainCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Camera.cpp: MainCharacter not found"));
	}

	// Find self position in world space
	cameraPos = this->GetActorLocation();
}

// Called every frame
void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FaceMainCharacter();
}

void ACamera::FaceMainCharacter()
{
	if (MainCharacter)
	{
		mainCharacterPos = MainCharacter->GetActorLocation();
	}

	//TODO: Make this smoother rather than snap to face, no idea how tho
	FVector Direction = mainCharacterPos - cameraPos;

	// Direction not distance
	Direction.Normalize();

	// Change foward vector to direction vector
	FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	this->SetActorRotation(NewRotation);
}

