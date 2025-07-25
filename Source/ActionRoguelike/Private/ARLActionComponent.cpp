// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLActionComponent.h"

#include "ARLAction.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


UARLActionComponent::UARLActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


void UARLActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Server Only
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UARLAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}


void UARLActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// Draw ALl Actions
	for (UARLAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
	
}


void UARLActionComponent::AddAction(AActor* Instigator, TSubclassOf<UARLAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	// Skip for clients
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempted to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	UARLAction* NewAction = NewObject<UARLAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}


void UARLActionComponent::RemoveAction(UARLAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning())) return;

	Actions.Remove(ActionToRemove);
}


bool UARLActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UARLAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				//FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			// Is Client
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			
			
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
		if (Action && Action->ActionName == ActionName && Action->IsRunning())
		{
			// Is Client
			if (!GetOwner()->HasAuthority())
			{
				ServerStopAction(Instigator, ActionName);
			}
			
			Action->StopAction(Instigator);
			return true;
		}
	}
	
	return false;
}

void UARLActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UARLActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

bool UARLActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UARLAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}

void UARLActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARLActionComponent, Actions);
}

