// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SP_Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASP_Character::ASP_Character()


{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SprinArmComp");
	SpringArmComp -> bUsePawnControlRotation = true;
	SpringArmComp -> SetupAttachment(RootComponent);

	SkateBoardMesh = CreateDefaultSubobject<UStaticMeshComponent>("SkateBoardMesh");
	SkateBoardMesh -> SetupAttachment(ASP_Character::GetMesh());

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp -> SetupAttachment(SpringArmComp);

	GetCharacterMovement() -> bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called to bind functionality to input
void ASP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// We need to get the local player controller
	const APlayerController* PC = GetController<APlayerController>();
	const ULocalPlayer* LP = PC -> GetLocalPlayer();

	// Initialize the system and check it
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LP -> GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
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

// Called every frame
void ASP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



