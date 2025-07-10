// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLProjectile.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AARLProjectile::AARLProjectile()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->OnComponentHit.AddDynamic(this, &AARLProjectile::OnActorHit);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AARLProjectile::OnActorOverlap);
	RootComponent = SphereComponent;

	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(SphereComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->InitialSpeed = 1000.0f;

	//AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
}

void AARLProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AARLProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		Explode();
	}
}


void AARLProjectile::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		Destroy();
	}
}

void AARLProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}






