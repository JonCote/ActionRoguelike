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
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	
}

// Called when the game starts or when spawned
void AARLCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	if (ensureAlways(PrimaryAttack_ProjectileClass))
	{
		PlayAnimMontage(AttackAnim);

		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AARLCharacter::PrimaryAttack_TimeElapsed, 0.2f);
	}
}

void AARLCharacter::PrimaryAttack_TimeElapsed()
{
	FVector CameraLocation = CameraComponent->GetComponentLocation();
	FVector CameraForwards = CameraComponent->GetForwardVector();
	FVector Start = CameraLocation + (CameraForwards * SpringArmComponent->TargetArmLength);
	FVector End = CameraLocation + (CameraForwards * 5000.f);
	bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(LookHitResult, Start, End, ECollisionChannel::ECC_Visibility);

	//FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	//DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0, 2.0f);
	
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(HandLocation, End);
	if (bBlockingHit)
	{
		Rot = UKismetMathLibrary::FindLookAtRotation(HandLocation, LookHitResult.ImpactPoint);
	}
	
	FTransform SpawnTM = FTransform(Rot, HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(PrimaryAttack_ProjectileClass, SpawnTM, SpawnParams);
}

void AARLCharacter::AbilityOne()
{
	if (ensureAlways(AbilityOne_ProjectileClass))
	{
		PlayAnimMontage(AttackAnim);

		GetWorldTimerManager().SetTimer(TimerHandle_AbilityOne, this, &AARLCharacter::AbilityOne_TimeElapsed, 0.2f);
	}
}

void AARLCharacter::AbilityOne_TimeElapsed()
{
	FVector CameraLocation = CameraComponent->GetComponentLocation();
	FVector CameraForwards = CameraComponent->GetForwardVector();
	FVector Start = CameraLocation + (CameraForwards * SpringArmComponent->TargetArmLength);
	FVector End = CameraLocation + (CameraForwards * 5000.f);
	bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(LookHitResult, Start, End, ECollisionChannel::ECC_Visibility);

	//FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	//DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0, 2.0f);
	
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(HandLocation, End);
	if (bBlockingHit)
	{
		Rot = UKismetMathLibrary::FindLookAtRotation(HandLocation, LookHitResult.ImpactPoint);
	}
	
	FTransform SpawnTM = FTransform(Rot, HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(AbilityOne_ProjectileClass, SpawnTM, SpawnParams);
}

void AARLCharacter::PrimaryInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}
