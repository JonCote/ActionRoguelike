// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLBlackholeProjectile.h"

#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AARLBlackholeProjectile::AARLBlackholeProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DestroySphereComponent = CreateDefaultSubobject<USphereComponent>("DestroySphereComponent");
	DestroySphereComponent->SetupAttachment(GetRootComponent());
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(GetRootComponent());

	RadialForceComponent->SetAutoActivate(false);

	RadialForceComponent->ForceStrength = -500.0f;
	RadialForceComponent->Radius = 1000.0f;
	RadialForceComponent->AddCollisionChannelToAffect(ECC_PhysicsBody);
}

// Called when the game starts or when spawned
void AARLBlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARLBlackholeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}