// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLGameModeBase.h"

#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AI/ARLAICharacter.h"


AARLGameModeBase::AARLGameModeBase()
{
	SpawnTimerInterval = 2.f;
}

void AARLGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AARLGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void AARLGameModeBase::SpawnBotTimerElapsed()
{
	int32 NumOfAliveBots = 0;
	for (TActorIterator<AARLAICharacter> It(GetWorld()); It; ++It)
	{
		AARLAICharacter* Bot = *It;
		if (Bot->IsAlive())
		{
			NumOfAliveBots++;
		}
	}

	float MaxBotCount = 1.f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if (NumOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AARLGameModeBase::OnQueryCompleted);
	}
	
}

void AARLGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"))
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}
