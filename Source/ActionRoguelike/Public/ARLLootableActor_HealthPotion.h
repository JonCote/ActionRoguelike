// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLLootableActor.h"
#include "ARLLootableActor_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLLootableActor_HealthPotion : public AARLLootableActor
{
	GENERATED_BODY()

public:

	AARLLootableActor_HealthPotion();

protected:
	
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditDefaultsOnly, Category="Potion")
	int32 HealthAmount;
};
