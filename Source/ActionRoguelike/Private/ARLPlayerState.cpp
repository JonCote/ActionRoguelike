// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPlayerState.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

int32 AARLPlayerState::GetCredits()
{
	return Credits;
}

void AARLPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f)) return;

	if (!HasAuthority())
	{
		ServerAddCredits(Delta);
		return;
	}
	
	Credits += Delta;
	OnRep_Credits(Credits - Delta);
}

void AARLPlayerState::ServerAddCredits_Implementation(int32 Delta)
{
	AddCredits(Delta);
}

bool AARLPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f)) return false;
	if (Credits < Delta) return false;

	AddCredits(-Delta);
	return true;
}

void AARLPlayerState::OnRep_Credits(int32 OldCredits)
{
	LogOnScreen(this, FString::Printf(TEXT("Credits: %d | Delta: %d"), Credits, Credits - OldCredits));
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void AARLPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARLPlayerState, Credits);
}
