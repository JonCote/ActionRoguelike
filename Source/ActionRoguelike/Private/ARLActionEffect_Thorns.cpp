// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLActionEffect_Thorns.h"

#include "ARLActionComponent.h"
#include "ARLAttributeComponent.h"
#include "ARLGameplayFunctionLibrary.h"
#include "ActionRoguelike/ActionRoguelike.h"

UARLActionEffect_Thorns::UARLActionEffect_Thorns()
{
	Period = 0.0f;
	Duration = 0.0f;
}

void UARLActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (UARLAttributeComponent* AttributeComponent = UARLAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner()))
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &UARLActionEffect_Thorns::OnHealthChanged);
	}
}

void UARLActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	
	if (UARLAttributeComponent* AttributeComponent = UARLAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner()))
	{
		AttributeComponent->OnHealthChanged.RemoveDynamic(this, &UARLActionEffect_Thorns::OnHealthChanged);
	}
}

void UARLActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();
	
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		int32 ReflectDamage = FMath::RoundToInt32(Delta);
		if (ReflectDamage == 0)
		{
			return;
		}
		ReflectDamage = FMath::Abs(ReflectDamage);

		/*LogOnScreen(this, FString::Printf(TEXT("Triggered: %s | OwningActor: %s | InstigatorActor: %s | ReflectDamage: %d"),
			*ActionName.ToString(),
			*OwningActor->GetName(),
			*InstigatorActor->GetName(),
			ReflectDamage),
			FColor::White);*/
		UARLGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectDamage);
	}
}
