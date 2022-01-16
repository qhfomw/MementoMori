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
	virtual void Interact(class AMoriCharacter* Interactor) {}

	virtual void BeginHover(class AMoriCharacter* Interactor) {}
	virtual void EndHover() {}

	virtual void BeginInteract(class AMoriCharacter* Interactor) {}
	virtual void EndInteract() {}

	virtual bool TryToInteract() { return false; }
	virtual float GetInteractionTime() const { return 0.f; }
	virtual const FString& GetDescription() const = 0;
	virtual const class UTexture2D* GetIcon() const { return nullptr; }
};
