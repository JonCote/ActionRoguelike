// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLTargetDummy.h"

// Sets default values
AARLTargetDummy::AARLTargetDummy()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;

	AttributeComponent = CreateDefaultSubobject<UARLAttributeComponent>("AttributeComponent");
	// Trigger when health is changed (damage/healing)
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AARLTargetDummy::OnHealthChanged);
	
}

void AARLTargetDummy::OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComponent->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}

