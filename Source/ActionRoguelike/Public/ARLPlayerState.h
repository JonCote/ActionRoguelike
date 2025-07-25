// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARLPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AARLPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="Credits")
	int32 GetCredits();

	UFUNCTION(BlueprintCallable, Category="Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category="Credits")
	bool RemoveCredits(int32 Delta);
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnCreditsChanged OnCreditsChanged;

protected:

	UFUNCTION(Server, Reliable)
	void ServerAddCredits(int32 Delta);

	UPROPERTY(ReplicatedUsing="OnRep_Credits", EditDefaultsOnly, Category="Credits")
	int32 Credits;
	
	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);
};



