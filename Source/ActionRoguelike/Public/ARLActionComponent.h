// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARLActionComponent.generated.h"


class UARLAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Actions")
	void AddAction(TSubclassOf<UARLAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);
	
	// Sets default values for this component's properties
	UARLActionComponent();

protected:

	/* Granted Abilities at game start */
	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<UARLAction>> DefaultActions;
	
	UPROPERTY()
	TArray<UARLAction*> Actions;
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
