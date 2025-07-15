// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLAction.h"
#include "ARLAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLAction_ProjectileAttack : public UARLAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere, Category="Attack")
	UParticleSystem* CastingEffect;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

	UARLAction_ProjectileAttack();
};
