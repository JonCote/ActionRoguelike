// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLActionEffect.h"
#include "ARLActionEffect_Thorns.generated.h"

class UARLAttributeComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLActionEffect_Thorns : public UARLActionEffect
{
	GENERATED_BODY()

public:

	UARLActionEffect_Thorns();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float Delta);

	
};

