#include "Weapon/MoriWeapon.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

AMoriWeapon::AMoriWeapon()
	: Socket(CreateDefaultSubobject<USceneComponent>(TEXT("Socket")))
	, Mesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh")))
	, ArmMesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh")))
{
	bReplicates = true;

	RootComponent = Socket;

	Mesh->SetupAttachment(RootComponent);
	Mesh->CanCharacterStepUpOn = ECB_No;
	Mesh->SetCollisionProfileName(TEXT("Weapon"));
	Mesh->CastShadow = false;
	Mesh->SetIsReplicated(true);
	
	ArmMesh->SetupAttachment(RootComponent);
	ArmMesh->SetCollisionProfileName(TEXT("NoCollision"));
	ArmMesh->CastShadow = false;
	ArmMesh->bLightAttachmentsAsGroup = true;
	ArmMesh->SetIsReplicated(true);
}
