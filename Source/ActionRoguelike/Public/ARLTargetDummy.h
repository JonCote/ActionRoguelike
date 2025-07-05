// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "ARLTargetDummy.generated.h"

class UARLAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AARLTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	

	AARLTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere)
	UARLAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float Delta);

};
