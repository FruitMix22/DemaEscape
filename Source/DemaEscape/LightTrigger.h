// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/TriggerVolume.h"
#include "LightTrigger.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEMAESCAPE_API ULightTrigger : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULightTrigger();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TurnOffLight();

	void GetAllValidActors(float DeltaTime);

private:
	UPROPERTY(EditAnywhere, Category = "Light")
	bool lightActive = true;

	UPROPERTY(EditAnywhere, Category = "Light")
	ATriggerVolume* TriggerVol;

	UPointLightComponent* pointLight;
		
};
