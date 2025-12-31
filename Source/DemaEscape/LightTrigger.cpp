// Fill out your copyright notice in the Description page of Project Settings.


#include "LightTrigger.h"

// Sets default values for this component's properties
ULightTrigger::ULightTrigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void ULightTrigger::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	pointLight = Owner->FindComponentByClass<UPointLightComponent>();

	if (pointLight)
	{
		pointLight->SetVisibility(lightActive);
	}
	
}


// Called every frame
void ULightTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetAllValidActors(DeltaTime);
}

void ULightTrigger::TurnOffLight()
{
	if (pointLight)
	{
		pointLight->SetVisibility(false);
		lightActive = false;
	}
}

void ULightTrigger::GetAllValidActors(float DeltaTime)
{
	TArray<AActor*> OverlappingActors;
	if (TriggerVol != nullptr && lightActive)
	{
		TriggerVol->GetOverlappingActors(OverlappingActors);

		for(AActor* Actor : OverlappingActors)
		{
			if (Actor->ActorHasTag("Player"))
			{
				TurnOffLight();
			}
		}
	}
}

