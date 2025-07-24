// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLLootableActor.h"

#include "Components/SphereComponent.h"

// Sets default values
AARLLootableActor::AARLLootableActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(Mesh);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AARLLootableActor::OnActorOverlap);

	SetReplicates(true);
}

void AARLLootableActor::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Destroy();
}


