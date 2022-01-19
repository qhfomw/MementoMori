#pragma once

#include "MoriActionInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MoriCharacter.generated.h"

UCLASS()
class MEMENTOMORI_API AMoriCharacter : public ACharacter, public IMoriActionInterface
{
	GENERATED_BODY()

public:
	AMoriCharacter();

	virtual void PickupWeapon(TSubclassOf<class AMoriPickedWeapon> WeaponClass, int32 AmmoNum) override;

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(Client, Reliable)
	void ClientInit();

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

	void Interacting();
	void StopInteracting();

	void TryToInteract();
	void CheckInteracting();

	void Interact();
	void BeginInteracting();
	void EndInteracting();

	UFUNCTION(Server, Reliable)
	void ServerInteracting(AActor* InteractedTarget, ACharacter* Interactor, bool bBeginning);

	UFUNCTION(Client, Reliable)
	void ClientStopInteracting();

	UFUNCTION(Server, Reliable)
	void ServerPickupWeapon(TSubclassOf<class AMoriPickedWeapon> WeaponClass, int32 AmmoNum);
	
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
	bool bTryToInteract = false;
	bool bInteracting = false;

	FTimerHandle CheckInteractionTimerHandle;
	FTimerHandle InteractionTimerHandle;

	UPROPERTY(Replicated)
	AActor* Target;
};
