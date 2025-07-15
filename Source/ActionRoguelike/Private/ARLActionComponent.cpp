// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLActionComponent.h"

#include "ARLAction.h"


UARLActionComponent::UARLActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UARLActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UARLAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}


void UARLActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UARLActionComponent::AddAction(TSubclassOf<UARLAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UARLAction* NewAction = NewObject<UARLAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}


bool UARLActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UARLAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}
	
	return false;
}


bool UARLActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UARLAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	
	return false;
}