// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SP_Character.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;


UCLASS()
class SP_BGSTASK_API ASP_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASP_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*CAMERA COMPONENTS*/
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComp;

	/*INPUT: MAPPING CONTEXT AND ACTIONS*/

	UPROPERTY(EditAnywhere, Category = "A_Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditAnywhere, Category = "A_Input")
	TObjectPtr<UInputAction> Input_Movement;
	
	UPROPERTY(EditAnywhere, Category = "A_Input")
	TObjectPtr<UInputAction> Input_Jump;

	UPROPERTY(EditAnywhere, Category = "A_Input")
	TObjectPtr<UInputAction> Input_LookMouse;

	/*Movement*/

	void Move(const FInputActionValue& Value);
	void LookMouse(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
