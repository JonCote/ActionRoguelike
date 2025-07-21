// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLLootableActor.h"
#include "ARLLootable_Credits.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLLootable_Credits : public AARLLootableActor
{
	GENERATED_BODY()

public:

	AARLLootable_Credits();

protected:

	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category="Credits")
	int32 CreditsAmount;

	
	
};
