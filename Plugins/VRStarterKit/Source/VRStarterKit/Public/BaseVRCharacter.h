#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseVRCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FInputBool, bool);

UCLASS(Blueprintable)
class VRSTARTERKIT_API ABaseVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseVRCharacter();

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void ToggleCrouch(bool doCrouch);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Tick(float DeltaSeconds) override;

	void UpdateRoomscaleAdjustment();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* VROrigin = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* LeftController = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* RightController = nullptr;
};

