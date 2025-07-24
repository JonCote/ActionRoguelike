// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLCharacter.h"

#include "ARLActionComponent.h"
#include "ARLAttributeComponent.h"
#include "ARLInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ARLInteractionComponent.h"
#include "Components/CapsuleComponent.h"
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
	ActionComponent =  CreateDefaultSubobject<UARLActionComponent>("ActionComponent");
	
	// true: Rotate Character towards movement direction
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	//~ true: Character rotates with camera
	//~ false: Camera only rotation
	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";
	
}

void AARLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &AARLCharacter::OnHealthChanged);
}

FVector AARLCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation() + (CameraComponent->GetForwardVector() * SpringArmComponent->TargetArmLength);
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
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AARLCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AARLCharacter::SprintEnd);
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

void AARLCharacter::SprintStart()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void AARLCharacter::SprintEnd()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void AARLCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void AARLCharacter::AbilityOne()
{
	ActionComponent->StartActionByName(this, "Blackhole");
}

void AARLCharacter::AbilityTwo()
{
	ActionComponent->StartActionByName(this, "Dash");
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

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();

		SetLifeSpan(5.0f);
		
	}

	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}
}

void AARLCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}
