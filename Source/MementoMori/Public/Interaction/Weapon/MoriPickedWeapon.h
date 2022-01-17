#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoriPickedWeapon.generated.h"

UCLASS()
class MEMENTOMORI_API AMoriPickedWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	AMoriPickedWeapon();

private:
	UPROPERTY()
	class USceneComponent* Socket;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* ArmMesh;
};
