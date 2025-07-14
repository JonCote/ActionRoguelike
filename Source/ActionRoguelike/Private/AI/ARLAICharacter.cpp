// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLAICharacter.h"

#include "AIController.h"
#include "ARLAttributeComponent.h"
#include "ARLWorldUserWidget.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AARLAICharacter::AARLAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AttributeComponent = CreateDefaultSubobject<UARLAttributeComponent>("AttributeComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TimeToHitParamName = "TimeToHit";
}

void AARLAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AARLAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AARLAICharacter::OnHealthChanged);
}

void AARLAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
}

void AARLAICharacter::OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UARLWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
		
		

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
		if (NewHealth <= 0.0f)
		{
			// Dead: Stop BT, ragdoll, and set lifespan
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			SetLifeSpan(10.0f);
			
			
		}
	}
}


void AARLAICharacter::SetTargetActor(AActor* NewTargetActor)
{
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTargetActor);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}




