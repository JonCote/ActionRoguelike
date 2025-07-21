// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLProjectile.h"
#include "GameplayTagContainer.h"
#include "ARLMagicProjectile.generated.h"


class UARLActionEffect;

UCLASS()
class ACTIONROGUELIKE_API AARLMagicProjectile : public AARLProjectile
{
	GENERATED_BODY()
	
public:
	
	AARLMagicProjectile();
	
	virtual void Tick(float DeltaTime) override;


protected:
	
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag ParryTag;

protected:

	UPROPERTY(EditDefaultsOnly, Category="Debuff")
	TSubclassOf<UARLActionEffect> DebuffActionClass;
	
};
