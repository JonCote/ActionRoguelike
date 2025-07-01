// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AARLExplosiveBarrel::AARLExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetCollisionProfileName("PhysicsActor");
	StaticMeshComponent->SetSimulatePhysics(true);
	RootComponent = StaticMeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);

	RadialForceComponent->SetAutoActivate(false);
	
	RadialForceComponent->ImpulseStrength = 2000.0f;
	RadialForceComponent->Radius = 700.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
	
}

// PostInitializeComponent runs before BeginPlay
void AARLExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AARLExplosiveBarrel::Explode);
}

void AARLExplosiveBarrel::Explode(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();
}

