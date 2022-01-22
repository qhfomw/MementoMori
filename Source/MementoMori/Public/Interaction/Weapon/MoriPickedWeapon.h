#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoriPickedWeapon.generated.h"

UENUM(BlueprintType)
enum class EMoriWeaponType : uint8
{
	Primary,
	Secondary,
	Melee,
	None
};

UCLASS()
class MEMENTOMORI_API AMoriPickedWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	AMoriPickedWeapon();

	EMoriWeaponType GetWeaponType() const { return WeaponType; }

private:
	UPROPERTY()
	class USceneComponent* Socket;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* ArmMesh;

	UPROPERTY(EditDefaultsOnly)
	EMoriWeaponType WeaponType = EMoriWeaponType::None;
};
