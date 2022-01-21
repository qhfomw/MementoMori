#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MoriActionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UMoriActionInterface : public UInterface
{
	GENERATED_BODY()
};

class MEMENTOMORI_API IMoriActionInterface
{
	GENERATED_BODY()

public:
	virtual void PickupWeapon(TSubclassOf<class AMoriPickedWeapon> WeaponClass, int32 AmmoNum) {}
};
