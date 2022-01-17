#include "Interaction/Weapon/MoriDroppedWeapon.h"
#include "Interaction/Weapon/MoriPickedWeapon.h"
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

void AMoriDroppedWeapon::MulticastInit_Implementation(TSubclassOf<AMoriPickedWeapon> InWeaponClass, float InAmmoNum, USkeletalMesh* InMesh)
{
	WeaponClass = InWeaponClass;
	AmmoNum = InAmmoNum;
	Mesh->SetSkeletalMesh(InMesh, true);

	if (HasAuthority() && GetWorld())
		GetWorld()->GetTimerManager().SetTimer(PhysicsTimerHandle, this, &AMoriDroppedWeapon::CheckPhysics, 1.f, true);
}

void AMoriDroppedWeapon::Interact(AMoriCharacter* Interactor)
{
	if (Interactor)
		Interactor->PickupWeapon(WeaponClass, AmmoNum);

	if (auto* World = GetWorld())
		World->DestroyActor(this);
}

void AMoriDroppedWeapon::CheckPhysics()
{
	if (Mesh->GetComponentVelocity() == FVector(0.f))
	{
		Mesh->SetSimulatePhysics(false);
		PhysicsTimerHandle.Invalidate();

		if (HasAuthority())
			SetReplicateMovement(false);
	}
}
