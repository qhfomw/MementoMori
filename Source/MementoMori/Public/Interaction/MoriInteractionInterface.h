#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MoriInteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UMoriInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class MEMENTOMORI_API IMoriInteractionInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(ACharacter* Interactor) {}

	virtual void BeginHover(ACharacter* Interactor) {}
	virtual void EndHover() {}

	virtual void BeginInteracting(ACharacter* Interactor) {}
	virtual void EndInteracting() {}

	virtual bool TryToInteract() { return false; }
	virtual float GetInteractionTime() const { return 0.f; }
	virtual const FString& GetDescription() const = 0;
	virtual const class UTexture2D* GetIcon() const { return nullptr; }
};
