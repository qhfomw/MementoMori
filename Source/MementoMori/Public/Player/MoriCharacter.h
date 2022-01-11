#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MoriCharacter.generated.h"

UCLASS()
class MEMENTOMORI_API AMoriCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMoriCharacter();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void Turn(float AxisValue);

	void TogglePerspective();
	void Jumping();
	void Crouching();
	void Running();
	void CheckRunning(bool bHaveToRun);

	UFUNCTION(Server, Reliable)
	void ServerRunning(bool bHaveToRun);
	
private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* ArmMesh;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	bool bUseFirstPersonView = true;
	bool bCrouching = false;
	bool bRunning = false;
};
