// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"

#include "ARLGameModeBase.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("arl.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);


// Sets default values for this component's properties
UARLAttributeComponent::UARLAttributeComponent()
{
	Health = 100;
	MaxHealth = 100;
}

bool UARLAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UARLAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool UARLAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}

float UARLAttributeComponent::GetHealthMax() const
{
	return MaxHealth;
}

bool UARLAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}
	
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	// Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		AARLGameModeBase* GM = GetWorld()->GetAuthGameMode<AARLGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
	return ActualDelta != 0.0f;
}

UARLAttributeComponent* UARLAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UARLAttributeComponent>(FromActor->GetComponentByClass(UARLAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UARLAttributeComponent::IsActorAlive(AActor* Actor)
{
	UARLAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}
	return false;
}

