// Copyright Epic Games, Inc. All Rights Reserved.

#include "WantedFPSCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WantedFPS.h"
#include "CurveBullet.h"
#include "InputCoreTypes.h"


AWantedFPSCharacter::AWantedFPSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void AWantedFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AWantedFPSCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AWantedFPSCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWantedFPSCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWantedFPSCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AWantedFPSCharacter::LookInput);
	}
	else
	{
		UE_LOG(LogWantedFPS, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	PlayerInputComponent->BindKey(
		EKeys::LeftMouseButton,
		IE_Pressed,
		this,
		&AWantedFPSCharacter::Fire
	);

	PlayerInputComponent->BindKey(
		EKeys::RightMouseButton,
		IE_Pressed,
		this,
		&AWantedFPSCharacter::StartCurveCharge
	);

	PlayerInputComponent->BindKey(
		EKeys::RightMouseButton,
		IE_Released,
		this,
		&AWantedFPSCharacter::StopCurveCharge
	);

	PlayerInputComponent->BindKey(
		EKeys::Q,
		IE_Pressed,
		this,
		&AWantedFPSCharacter::SetCurveLeft
	);

	PlayerInputComponent->BindKey(
		EKeys::E,
		IE_Pressed,
		this,
		&AWantedFPSCharacter::SetCurveRight
	);
}


void AWantedFPSCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AWantedFPSCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AWantedFPSCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AWantedFPSCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AWantedFPSCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AWantedFPSCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void AWantedFPSCharacter::Fire()
{
	UCameraComponent* Camera = FindComponentByClass<UCameraComponent>();

	if (!Camera || !GetWorld())
	{
		return;
	}

	const FVector SpawnLocation =
		Camera->GetComponentLocation()
		+ Camera->GetForwardVector() * 100.0f;

	const FRotator SpawnRotation =
		Camera->GetComponentRotation();

	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ACurveBullet* Bullet = GetWorld()->SpawnActor<ACurveBullet>(
		ACurveBullet::StaticClass(),
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (!Bullet)
	{
		return;
	}

	// カーブの最大チャージ時間
	const float MaxChargeTime = 0.5f;

	float CurveStrength = 0.0f;

	if (bIsCurveCharging)
	{
		const float ChargeTime = GetCurveChargeTime();

		CurveStrength = FMath::Clamp(
			ChargeTime / MaxChargeTime,
			0.0f,
			1.0f
		);
	}

	Bullet->SetCurveStrength(CurveStrength);
	Bullet->SetCurveDirection(CurveDirection);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Fire - CurveStrength: %.2f"),
		CurveStrength
	);
}

void AWantedFPSCharacter::StartCurveCharge()
{
	if (!GetWorld())
	{
		return;
	}

	bIsCurveCharging = true;
	CurveChargeStartTime = GetWorld()->GetTimeSeconds();

	if (CurveDirection == -1.0f && CurveLeftChargeMontage && FirstPersonMesh)
	{
		if (UAnimInstance* FirstPersonAnimInstance = FirstPersonMesh->GetAnimInstance())
		{
			FirstPersonAnimInstance->Montage_Play(CurveLeftChargeMontage);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Curve charge started"));
}

void AWantedFPSCharacter::StopCurveCharge()
{
	if (!bIsCurveCharging)
	{
		return;
	}

	const float ChargeTime = GetCurveChargeTime();

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Curve charge stopped: %.2f sec"),
		ChargeTime
	);

	bIsCurveCharging = false;
}

float AWantedFPSCharacter::GetCurveChargeTime() const
{
	if (!bIsCurveCharging || !GetWorld())
	{
		return 0.0f;
	}

	return GetWorld()->GetTimeSeconds() - CurveChargeStartTime;
}

void AWantedFPSCharacter::SetCurveLeft()
{
	CurveDirection = -1.0f;

	UE_LOG(LogTemp, Warning, TEXT("Curve Direction: Left"));
}

void AWantedFPSCharacter::SetCurveRight()
{
	CurveDirection = 1.0f;

	UE_LOG(LogTemp, Warning, TEXT("Curve Direction: Right"));
}