// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARLExplosiveBarrel.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API AARLExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:

	AARLExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URadialForceComponent* RadialForceComponent;
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Explode(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
