// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPlayerState.h"

int32 AARLPlayerState::GetCredits()
{
	return Credits;
}

void AARLPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f)) return;

	Credits += Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool AARLPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f)) return false;
	if (Credits < Delta) return false;

	Credits -= Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
	return true;
}
