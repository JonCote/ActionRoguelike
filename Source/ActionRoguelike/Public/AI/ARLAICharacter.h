// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARLAICharacter.generated.h"

class UPawnSensingComponent;
class UARLAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AARLAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARLAICharacter();

	UFUNCTION()
	bool IsAlive();

protected:

	void SetTargetActor(AActor* NewTargetActor);

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UARLAttributeComponent* AttributeComponent;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float Delta);

	
	
};