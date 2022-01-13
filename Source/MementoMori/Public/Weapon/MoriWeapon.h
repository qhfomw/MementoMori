#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoriWeapon.generated.h"

UCLASS()
class MEMENTOMORI_API AMoriWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	AMoriWeapon();

private:
	UPROPERTY()
	class USceneComponent* Socket;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* ArmMesh;
};
