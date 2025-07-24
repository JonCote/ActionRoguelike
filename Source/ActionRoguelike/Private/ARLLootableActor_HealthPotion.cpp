// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLLootableActor_HealthPotion.h"

#include "ARLGameplayFunctionLibrary.h"
#include "ARLPlayerState.h"

AARLLootableActor_HealthPotion::AARLLootableActor_HealthPotion()
{
	HealthAmount = 20;
}

void AARLLootableActor_HealthPotion::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (UARLGameplayFunctionLibrary::ApplyHealing(this, OtherActor, HealthAmount))
		{
			Destroy();
		}
	}
}
