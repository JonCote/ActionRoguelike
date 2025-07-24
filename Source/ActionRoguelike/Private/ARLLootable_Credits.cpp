// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLLootable_Credits.h"

#include "ARLCharacter.h"
#include "ARLPlayerState.h"

AARLLootable_Credits::AARLLootable_Credits()
{
	CreditsAmount = 80;

	SetReplicates(true);
}

void AARLLootable_Credits::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* Pawn = Cast<APawn>(OtherActor))
	{
		if (!Pawn->HasAuthority()) return;
		AARLPlayerState* PlayerState = Pawn->GetPlayerState<AARLPlayerState>();
		if (PlayerState)
		{
			PlayerState->AddCredits(CreditsAmount);
			Destroy();
		}
	}
}
