#pragma once

#include "../MoriInteractionInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoriDroppedWeapon.generated.h"

UCLASS()
class MEMENTOMORI_API AMoriDroppedWeapon : public AActor, public IMoriInteractionInterface
{
	GENERATED_BODY()

public:
	AMoriDroppedWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastInit(TSubclassOf<class AMoriPickedWeapon> InWeaponClass, float InAmmoNum, class USkeletalMesh* InMesh);

	virtual void Interact(class AMoriCharacter* Interactor) override;

	virtual float GetInteractionTime() const { return InteractionTime; }
	virtual const FString& GetDescription() const { return Description; }
	virtual const class UTexture2D* GetIcon() const { return Icon; }

private:
	void CheckPhysics();

private:
	UPROPERTY(EditInstanceOnly)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly)
	float InteractionTime;

	UPROPERTY(EditInstanceOnly)
	FString Description;

	UPROPERTY(EditInstanceOnly)
	class UTexture2D* Icon;

	UPROPERTY(EditInstanceOnly)
	TSubclassOf<class AMoriPickedWeapon> WeaponClass;

	UPROPERTY(EditInstanceOnly)
	float AmmoNum;

	FTimerHandle PhysicsTimerHandle;
};
