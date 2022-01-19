#include "Player/MoriCharacter.h"
#include "Interaction/MoriInteractionInterface.h"
#include "Interaction/Weapon/MoriDroppedWeapon.h"
#include "Interaction/Weapon/MoriPickedWeapon.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AMoriCharacter::AMoriCharacter()
	: SpringArm(CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm")))
	, Camera(CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")))
	, ArmMesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh")))
{
	bAlwaysRelevant = true;
	NetUpdateFrequency = 60.f;
	NetPriority = 4.f;
	SetCanBeDamaged(false);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetCollisionProfileName(TEXT("Pawn"));
	GetMesh()->SetOwnerNoSee(true);

	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(20.f, 0.f, 70.f));
	SpringArm->TargetArmLength = 0.f;
	SpringArm->ProbeSize = 5.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 20.f;
	
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeScale3D(FVector(0.25f));

	ArmMesh->SetupAttachment(Camera);
	ArmMesh->SetRelativeLocation(FVector(4.f, 0.f, -165.5f));
	ArmMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	ArmMesh->bSelfShadowOnly = true;
	ArmMesh->bLightAttachmentsAsGroup = true;
	ArmMesh->SetOnlyOwnerSee(true);

	GetCharacterMovement()->MaxWalkSpeed = 360.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 170.f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void AMoriCharacter::PickupWeapon(TSubclassOf<AMoriPickedWeapon> WeaponClass, int32 AmmoNum)
{
	ServerPickupWeapon(WeaponClass, AmmoNum);
}

void AMoriCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ClientInit();
}

void AMoriCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMoriCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMoriCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMoriCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMoriCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("TogglePerspective"), IE_Pressed, this, &AMoriCharacter::TogglePerspective);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMoriCharacter::Jumping);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AMoriCharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AMoriCharacter::Crouching);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &AMoriCharacter::Running);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &AMoriCharacter::Running);

	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AMoriCharacter::Interacting);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Released, this, &AMoriCharacter::StopInteracting);
}

void AMoriCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMoriCharacter, Target);
}

void AMoriCharacter::ClientInit_Implementation()
{
	if (auto* World = GetWorld())
		World->GetTimerManager().SetTimer(CheckInteractionTimerHandle, this, &AMoriCharacter::CheckInteracting, 0.1f, true);
}

void AMoriCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void AMoriCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void AMoriCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AMoriCharacter::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AMoriCharacter::TogglePerspective()
{
	bUseFirstPersonView = !bUseFirstPersonView;
	
	GetMesh()->SetOwnerNoSee(bUseFirstPersonView);
	ArmMesh->SetVisibility(bUseFirstPersonView);
	
	SpringArm->TargetArmLength = bUseFirstPersonView ? 0.f : 200.f;
	SpringArm->SetRelativeLocation(FVector(20.f, (bUseFirstPersonView ? 0.f : 35.f), 70.f));
}

void AMoriCharacter::Jumping()
{
	bCrouching ? UnCrouch() : Jump();
	bCrouching = false;
}

void AMoriCharacter::Crouching()
{
	if (GetCharacterMovement()->IsFalling())
		return;

	bCrouching = !bCrouching;
	bCrouching ? Crouch() : UnCrouch();

	if (bRunning)
		CheckRunning(!bCrouching);
}

void AMoriCharacter::Running()
{
	bRunning = !bRunning;

	if (bRunning && bCrouching)
	{
		bCrouching = false;
		UnCrouch();
	}

	CheckRunning(bRunning);
}

void AMoriCharacter::CheckRunning(bool bHaveToRun)
{
	GetCharacterMovement()->MaxWalkSpeed = bHaveToRun ? 540.f : 360.f;

	if (UGameplayStatics::GetPlayerCameraManager(this, 0) && CameraShakeClass)
	{
		auto* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
		bHaveToRun ? CameraManager->StartCameraShake(CameraShakeClass) : CameraManager->StopAllCameraShakes();
	}

	ServerRunning(bHaveToRun);
}

void AMoriCharacter::ServerRunning_Implementation(bool bHaveToRun)
{
	GetCharacterMovement()->MaxWalkSpeed = bHaveToRun ? 540.f : 360.f;
}

void AMoriCharacter::Interacting()
{
	bTryToInteract = true;

	if (Target)
		TryToInteract();
}

void AMoriCharacter::StopInteracting()
{
	bTryToInteract = false;

	if (Target)
		EndInteracting();
}

void AMoriCharacter::TryToInteract()
{
	if (auto* InteractedTarget = Cast<IMoriInteractionInterface>(Target))
	{
		if (!InteractedTarget->TryToInteract())
			return;

		BeginInteracting();

		if (auto* World = GetWorld())
		{
			World->GetTimerManager().PauseTimer(CheckInteractionTimerHandle);
			InteractedTarget->GetInteractionTime() ? World->GetTimerManager().SetTimer(InteractionTimerHandle, this, &AMoriCharacter::Interact, InteractedTarget->GetInteractionTime()) : Interact();
		}
	}
}

void AMoriCharacter::CheckInteracting()
{
	static AActor* PrevTarget = nullptr;

	if (bRunning)
		PrevTarget = Target = nullptr;

	else if (auto* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
	{
		FHitResult OutHit;
		const auto& Start = CameraManager->GetCameraLocation() + (bUseFirstPersonView ? FVector(0.f) : CameraManager->GetActorForwardVector() * (CameraManager->GetCameraLocation() - GetActorLocation()).Size());
		
		if (UKismetSystemLibrary::LineTraceSingle(this, Start, Start + CameraManager->GetActorForwardVector() * 300.f, ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>{}, EDrawDebugTrace::None, OutHit, true))
		{
			if (auto* InteractedTarget = Cast<IMoriInteractionInterface>(OutHit.GetActor()))
			{
				if (InteractedTarget->TryToInteract())
				{
					Target = OutHit.GetActor();

					if (bTryToInteract)
						TryToInteract();

					if (Target != PrevTarget)
					{
						if (auto* PrevInteractedTarget = Cast<IMoriInteractionInterface>(PrevTarget))
							PrevInteractedTarget->EndHover();

						PrevTarget = Target;
					}
					
					return;
				}
			}
		}

		if (auto* InteractedTarget = Cast<IMoriInteractionInterface>(Target))
		{
			InteractedTarget->EndHover();
			EndInteracting();

			PrevTarget = Target = nullptr;
		}
	}
}

void AMoriCharacter::Interact()
{
	ServerInteracting(Target, this, false);

	bTryToInteract = bInteracting = false;
	Target = nullptr;
	
	if (auto* World = GetWorld())
		World->GetTimerManager().UnPauseTimer(CheckInteractionTimerHandle);
}

void AMoriCharacter::BeginInteracting()
{
	ServerInteracting(Target, this, true);

	bTryToInteract = false;
	bInteracting = true;
}

void AMoriCharacter::EndInteracting()
{
	bInteracting = false;

	if (auto* InteractedTarget = Cast<IMoriInteractionInterface>(Target))
		InteractedTarget->EndInteracting();

	if (auto* World = GetWorld())
	{
		World->GetTimerManager().PauseTimer(InteractionTimerHandle);
		World->GetTimerManager().UnPauseTimer(CheckInteractionTimerHandle);
	}
}

void AMoriCharacter::ServerInteracting_Implementation(AActor* InteractedTarget, ACharacter* Interactor, bool bBeginning)
{
	if (auto* Interactable = Cast<IMoriInteractionInterface>(InteractedTarget))
		bBeginning ? Interactable->BeginInteracting(Interactor) : Interactable->Interact(Interactor);
}

void AMoriCharacter::ClientStopInteracting_Implementation()
{
	if (bInteracting)
		EndInteracting();
}

void AMoriCharacter::ServerPickupWeapon_Implementation(TSubclassOf<AMoriPickedWeapon> WeaponClass, int32 AmmoNum)
{

}
