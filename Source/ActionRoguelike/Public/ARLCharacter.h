// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARLCharacter.generated.h"

class UARLActionComponent;
class UARLAttributeComponent;
class UARLInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API AARLCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	UARLInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UARLAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UARLActionComponent* ActionComponent;
	

	void MoveForward(float val);

	void MoveRight(float val);

	void SprintStart();
	
	void SprintEnd();

	void PrimaryAttack();

	void AbilityOne();

	void AbilityTwo();

	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

public:	
	
	AARLCharacter();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount=100);
	
};


