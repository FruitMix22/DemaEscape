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

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player Location currently not known.")));
	}

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	if (MainCharacter)
	{
		mainCharacterPos = MainCharacter->GetActorLocation();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player Found!")));
		}

	}
}

// Called every frame
void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GEngine)
	{
		mainCharacterPos = MainCharacter->GetActorLocation();
		UE_LOG(LogTemp, Display, TEXT("Player Location: X=%f Y=%f Z=%f"), mainCharacterPos.X, mainCharacterPos.Y, mainCharacterPos.Z);
	}

}

