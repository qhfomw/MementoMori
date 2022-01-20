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

	UFUNCTION(Server, Reliable)
	void ServerInit(class USkeletalMesh* InMesh, TSubclassOf<class AMoriPickedWeapon> InWeaponClass, int32 InAmmoNum);

	virtual void Interact(ACharacter* Interactor) override;

	virtual bool TryToInteract() { return bTryToInteract; }
	virtual float GetInteractionTime() const { return InteractionTime; }
	virtual const FString& GetDescription() const { return Description; }
	virtual const class UTexture2D* GetIcon() const { return Icon; }

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated, VisibleInstanceOnly)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(Replicated, EditInstanceOnly)
	TSubclassOf<class AMoriPickedWeapon> WeaponClass;

	UPROPERTY(Replicated, EditInstanceOnly)
	int32 AmmoNum;

	UPROPERTY(EditInstanceOnly)
	bool bTryToInteract = true;

	UPROPERTY(EditInstanceOnly)
	float InteractionTime;

	UPROPERTY(EditInstanceOnly)
	FString Description;

	UPROPERTY(EditInstanceOnly)
	class UTexture2D* Icon;
};
