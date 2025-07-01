// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLCharacter.h"

#include "ARLInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ARLInteractionComponent.h"

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

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AARLCharacter::PrimaryAttack_TimeElapsed, 0.2f);
	
}

void AARLCharacter::PrimaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void AARLCharacter::PrimaryInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}
