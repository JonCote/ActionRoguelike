// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARLAICharacter.generated.h"

class UARLActionComponent;
class UARLWorldUserWidget;
class UPawnSensingComponent;
class UARLAttributeComponent;
class UUserWidget;

UCLASS()
class ACTIONROGUELIKE_API AARLAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARLAICharacter();

protected:

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float Delta);

	void SetTargetActor(AActor* NewTargetActor);

	virtual void PostInitializeComponents() override;
	

protected:

	UARLWorldUserWidget* ActiveHealthBar;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName TimeToHitParamName;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UARLAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UARLActionComponent* ActionComponent;

	

	
	
};