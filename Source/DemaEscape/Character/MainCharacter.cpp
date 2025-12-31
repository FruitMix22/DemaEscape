// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 5.f;
	SpringArm->bUsePawnControlRotation = true;

	// Head bob pivot
	HeadBobPivot = CreateDefaultSubobject<USceneComponent>(TEXT("HeadBobPivot"));
	HeadBobPivot->SetupAttachment(SpringArm);

	// Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(HeadBobPivot);
	FollowCamera->bUsePawnControlRotation = false;

	// Assign footstep sound cue
	static ConstructorHelpers::FObjectFinder<USoundCue> footstepCue(TEXT("/Game/Audio/Footsteps/SC_FootSteps"));
	footstepAudioCue = footstepCue.Object;

	// Create audio component for footstep sounds
	footstepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootstepAudioComponent"));
	footstepAudioComponent->SetupAttachment(GetCapsuleComponent());
	footstepAudioComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add input mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
				(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
	}

	if (HeadBobPivot)
	{
		CameraStartLocation = HeadBobPivot->GetRelativeLocation();
	}

	if (footstepAudioComponent->IsValidLowLevelFast()) {
		footstepAudioComponent->SetSound(footstepAudioCue);
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HeadBobPivot) return;

	float Speed = GetVelocity().Size2D();

	if (Speed > 5.f && GetCharacterMovement()->IsMovingOnGround())
	{
		HeadBobTime += DeltaTime * HeadBobFrequency;

		float BobOffset = FMath::Sin(HeadBobTime) * HeadBobAmplitude;
		UE_LOG(LogTemp, Warning, TEXT("BobOffset: %f"), BobOffset);

		

		FVector NewLocation = CameraStartLocation;
		NewLocation.Z += BobOffset;

		HeadBobPivot->SetRelativeLocation(NewLocation);
		if( BobOffset < 0.f && previousHeadBobOffset >= 0.f )
		{
			footstepAudioComponent->Play();
		}
		previousHeadBobOffset = BobOffset;
	}
	else
	{
		HeadBobTime = 0.f;

		FVector Current = HeadBobPivot->GetRelativeLocation();
		HeadBobPivot->SetRelativeLocation(
			FMath::VInterpTo(Current, CameraStartLocation, DeltaTime, 10.f)
		);
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = 
			CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::Jumping);
		
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
	}

}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMainCharacter::Jumping()
{
	// No jumping for now, might come back later
	//Jump();
	footstepAudioComponent->Play();
}

