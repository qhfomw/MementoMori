#include "Interaction/Weapon/MoriDroppedWeapon.h"
#include "Interaction/Weapon/MoriPickedWeapon.h"
#include "Player/MoriActionInterface.h"
#include "Player/MoriCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

AMoriDroppedWeapon::AMoriDroppedWeapon()
	: Mesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh")))
{
	bAlwaysRelevant = true;
	bReplicates = true;
	SetReplicateMovement(true);

	Mesh->SetCollisionProfileName(TEXT("DroppedWeapon"));
	Mesh->SetSimulatePhysics(true);
}

void AMoriDroppedWeapon::ServerInit_Implementation(USkeletalMesh* InMesh, TSubclassOf<AMoriPickedWeapon> InWeaponClass, int32 InAmmoNum)
{
	Mesh->SetSkeletalMesh(InMesh, true);
	WeaponClass = InWeaponClass;
	AmmoNum = InAmmoNum;
}

void AMoriDroppedWeapon::Interact(ACharacter* Interactor)
{
	if (auto* Picker = Cast<IMoriActionInterface>(Interactor))
		Picker->PickupWeapon(WeaponClass, AmmoNum);

	if (auto* World = GetWorld())
		World->DestroyActor(this);
}

void AMoriDroppedWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMoriDroppedWeapon, Mesh);
	DOREPLIFETIME(AMoriDroppedWeapon, WeaponClass);
	DOREPLIFETIME(AMoriDroppedWeapon, AmmoNum);
}
