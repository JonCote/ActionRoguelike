// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARLCharacter.generated.h"

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
	FName HandSocketName;

	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName TimeToHitParamName;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> PrimaryAttack_ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> AbilityOne_ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> AbilityTwo_ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackAnimDelay = 0.2f;
	
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_AbilityOne;
	FTimerHandle TimerHandle_AbilityTwo;

	FHitResult LookHitResult;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	UARLInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UARLAttributeComponent* AttributeComponent;

	void MoveForward(float val);

	void MoveRight(float val);

	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void AbilityOne();

	void AbilityOne_TimeElapsed();

	void AbilityTwo();

    void AbilityTwo_TimeElapsed();

	void PrimaryInteract();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

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


