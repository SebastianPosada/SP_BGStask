// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SP_Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/SP_PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASP_Character::ASP_Character()


{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and initialize all needed components
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SprinArmComp");
	SpringArmComp -> bUsePawnControlRotation = true;
	SpringArmComp -> SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 500.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp -> SetupAttachment(SpringArmComp);
	
	SkateBoardMesh = CreateDefaultSubobject<UStaticMeshComponent>("SkateBoardMesh");
	SkateBoardMesh -> SetupAttachment(ASP_Character::GetMesh());

	GetCharacterMovement() -> bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called to bind functionality to input
void ASP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// We need to get the local player controller
	const APlayerController* PlayerController = GetController<APlayerController>();
	const ULocalPlayer* LocalPlayer = PlayerController -> GetLocalPlayer();
	
	// Initialize the system and check it
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer -> GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(InputSubsystem);

	// Clear All Mappings
	InputSubsystem -> ClearAllMappings();

	// Add Mappings for our task
	InputSubsystem -> AddMappingContext(InputContext, 0);

	// New Enhanced Input System
	UEnhancedInputComponent* EnhancedInputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Input Actions
	EnhancedInputComp -> BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComp -> BindAction(Input_Movement, ETriggerEvent::Triggered, this, &ASP_Character::Move);
	EnhancedInputComp -> BindAction(Input_Pause, ETriggerEvent::Triggered, this, &ASP_Character::TogglePause);
	EnhancedInputComp -> BindAction(Input_Sprint, ETriggerEvent::Started, this, &ASP_Character::ToggleSprintOn);
	EnhancedInputComp -> BindAction(Input_Sprint, ETriggerEvent::Completed, this, &ASP_Character::ToggleSprintOff);
	
	// MKB
	EnhancedInputComp -> BindAction(Input_LookMouse, ETriggerEvent::Triggered, this, &ASP_Character::LookMouse);
}

// Called when the game starts or when spawned
void ASP_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ASP_Character::Move(const FInputActionValue& Value)
{
	// gets the control rotation and sets pitch and roll to 0
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// get the value from input (Combined value from WASD keys or single GamePad Stick) and convert it to Vector2d (x, y)
	const FVector2d AxisValue = Value.Get<FVector2d>();

	// Move Forwards / Backwards
	AddMovementInput(ControlRot.Vector(), AxisValue.Y);

	// Move Right / Left strafe
	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, AxisValue.X);	
}

void ASP_Character::LookMouse(const FInputActionValue& Value)
{
	// get the value from input (Combined value from WASD keys or single GamePad Stick) and convert it to Vector2d (x, y)
	const FVector2d InputValue = Value.Get<FVector2d>();

	AddControllerYawInput(InputValue.X);	
	AddControllerPitchInput(InputValue.Y);
}

void ASP_Character::TogglePause()
{
	// Gets the current player controller and calls its pause function
	ASP_PlayerController* SP_PlayerController = CastChecked<ASP_PlayerController>(GetController<APlayerController>());
	SP_PlayerController->TogglePause();	
}

void ASP_Character::ToggleSprintOn()
{
	if(CameraFOVTimerHandle.IsValid())
	{
		CameraFOVTimerHandle.Invalidate();
	}

	GetWorld()->GetTimerManager().SetTimer(CameraFOVTimerHandle, this, &ASP_Character::ChangeFOV, 0.01f,false);
	GetCharacterMovement() -> MaxWalkSpeed = 1800.0f;
	PlayCustomAnimationMontage(SprintMontage, true);
}

void ASP_Character::ToggleSprintOff()
{
	if(CameraFOVTimerHandle.IsValid())
	{
		CameraFOVTimerHandle.Invalidate();
	}
	
	GetWorld()->GetTimerManager().SetTimer(CameraFOVTimerHandle, this, &ASP_Character::ChangeFOV, 0.01f,false);
	GetCharacterMovement() -> MaxWalkSpeed = 600.0f;
	PlayCustomAnimationMontage(SprintMontage, false);
}

void ASP_Character::PlayCustomAnimationMontage(UAnimMontage* AnimMontage, bool bMontagePlaying)
{
	// Get the animation instance of the mesh and, if both anim instance and anim montage are valid, plays the montage
	UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();

	// Validates that AnimInstance and Anim Montage are Valid
	if (AnimInstance && AnimMontage)
	{
		// Validate whether the AnimMontage is playing, and based on input will play or stom the montage
		if(bMontagePlaying && !AnimInstance->Montage_IsPlaying(AnimMontage))
		{
			AnimInstance->Montage_Play(AnimMontage);
		}
		else if (!bMontagePlaying && AnimInstance->Montage_IsPlaying(AnimMontage))
		{
			AnimInstance->Montage_Stop(.5, AnimMontage);
		}
	}
}

void ASP_Character::ChangeFOV()
{
	float TargetFOV;
	
	if(bIsSprintActive)
	{
		TargetFOV = 90.0f;
		bIsSprintActive = false;
	}
	else
	{
		TargetFOV = 120.0f;
		bIsSprintActive = true;
	}
	
	float InterpSpeed = 1.0f;
	float CurrentFOV = CameraComp->FieldOfView;
	float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, 0.01, InterpSpeed);
	CameraComp->SetFieldOfView(NewFOV);
}

// Called every frame
void ASP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



