// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLProjectile.h"
#include "ARLBlackholeProjectile.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API AARLBlackholeProjectile : public AARLProjectile
{
	GENERATED_BODY()

public:
	
	AARLBlackholeProjectile();

protected:

	UPROPERTY(VisibleAnywhere)
	USphereComponent* DestroySphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URadialForceComponent* RadialForceComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
