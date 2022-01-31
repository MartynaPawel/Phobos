#include "BaseVRCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MotionControllerComponent.h"

ABaseVRCharacter::ABaseVRCharacter()
{
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VR Origin"));
	VROrigin->SetWorldLocation(FVector(0, 0, -90));
	VROrigin->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Camera->SetWorldLocation(FVector(0, 0, 70));
	Camera->SetupAttachment(VROrigin);
	Camera->bUsePawnControlRotation = false;

	GetMesh()->SetOwnerNoSee(true);

	Cast<UCharacterMovementComponent>(GetMovementComponent())->NavAgentProps.bCanCrouch = true;

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Controller"));
	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Controller"));
	LeftController->MotionSource = TEXT("Left");
	RightController->MotionSource = TEXT("Right");
	LeftController->SetupAttachment(VROrigin);
	RightController->SetupAttachment(VROrigin);
}

void ABaseVRCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	//Bind Axis
	PlayerInputComponent->BindAxis("Move_Y", this, &ABaseVRCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move_X", this, &ABaseVRCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Look_X", this, &ABaseVRCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look_Y", this, &ABaseVRCharacter::AddControllerPitchInput);

	//Bind Actions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseVRCharacter::Jump);
	PlayerInputComponent->BindAction<FInputBool>("Crouch", IE_Pressed, this, &ABaseVRCharacter::ToggleCrouch, true);
	PlayerInputComponent->BindAction<FInputBool>("Crouch", IE_Released, this, &ABaseVRCharacter::ToggleCrouch, false);
}

void ABaseVRCharacter::MoveForward(float value)
{
	FVector dir = Camera->GetForwardVector() * FVector(1, 1, 0);
	dir.Normalize();
	AddMovementInput(dir, value);
}

void ABaseVRCharacter::MoveRight(float value)
{
	FVector dir = Camera->GetRightVector() * FVector(1, 1, 0);
	dir.Normalize();
	AddMovementInput(dir, value);
}

void ABaseVRCharacter::ToggleCrouch(bool doCrouch)
{
	if (doCrouch) Crouch();
	else UnCrouch();
}

void ABaseVRCharacter::Tick(float DeltaSeconds)
{
	UpdateRoomscaleAdjustment();
}

void ABaseVRCharacter::UpdateRoomscaleAdjustment()
{
	const auto deltaLocation = (Camera->GetComponentLocation() - GetCapsuleComponent()->GetComponentLocation()) * FVector(1, 1, 0);
	AddActorWorldOffset(deltaLocation);
	VROrigin->AddWorldOffset(-deltaLocation);
}