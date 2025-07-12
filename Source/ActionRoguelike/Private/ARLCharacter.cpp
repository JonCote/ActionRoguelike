// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLCharacter.h"

#include "ARLAttributeComponent.h"
#include "ARLInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ARLInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AARLCharacter::AARLCharacter()
{
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComponent = CreateDefaultSubobject<UARLInteractionComponent>("InteractionComponent");
	AttributeComponent = CreateDefaultSubobject<UARLAttributeComponent>("AttributeComponent");
	
	// true: Rotate Character towards movement direction
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	//~ true: Character rotates with camera
	//~ false: Camera only rotation
	bUseControllerRotationYaw = false;

	HandSocketName = "Muzzle_01";
	
}

void AARLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &AARLCharacter::OnHealthChanged);
}

// Called to bind functionality to input
void AARLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//~ Input Mapping has been deprecated replace with Enhanced Input system later
	PlayerInputComponent->BindAxis("MoveForward", this, &AARLCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AARLCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AARLCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("AbilityOne", IE_Pressed, this, &AARLCharacter::AbilityOne);
	PlayerInputComponent->BindAction("AbilityTwo", IE_Pressed, this, &AARLCharacter::AbilityTwo);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AARLCharacter::PrimaryInteract);
	//~
}

void AARLCharacter::MoveForward(float val)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), val);
}

void AARLCharacter::MoveRight(float val)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, val);
}

void AARLCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AARLCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay);
	
}

void AARLCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(PrimaryAttack_ProjectileClass);
}

void AARLCharacter::AbilityOne()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_AbilityOne, this, &AARLCharacter::AbilityOne_TimeElapsed, AttackAnimDelay);
}

void AARLCharacter::AbilityOne_TimeElapsed()
{
	SpawnProjectile(AbilityOne_ProjectileClass);
}

void AARLCharacter::AbilityTwo()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_AbilityTwo, this, &AARLCharacter::AbilityTwo_TimeElapsed, AttackAnimDelay);
}

void AARLCharacter::AbilityTwo_TimeElapsed()
{
	SpawnProjectile(AbilityTwo_ProjectileClass);
}

void AARLCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector CameraLocation = CameraComponent->GetComponentLocation();
		FVector CameraForwards = CameraComponent->GetForwardVector();
		FVector TraceStart = CameraLocation + (CameraForwards * SpringArmComponent->TargetArmLength);
		FVector TraceEnd = CameraLocation + (CameraForwards * 5000.f);

		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

void AARLCharacter::PrimaryInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

void AARLCharacter::OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}

	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}
