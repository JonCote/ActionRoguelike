// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAction.h"

#include "ARLActionComponent.h"

void UARLAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	UARLActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UARLAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));
	
	ensureAlways(bIsRunning);

	UARLActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

bool UARLAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning()) return false;
	
	UARLActionComponent* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

bool UARLAction::IsRunning() const
{
	return bIsRunning;
}

UWorld* UARLAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}

UARLActionComponent* UARLAction::GetOwningComponent() const
{
	return Cast<UARLActionComponent>(GetOuter());
}
