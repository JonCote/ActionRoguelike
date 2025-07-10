// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AARLAICharacter::AARLAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	
}

void AARLAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AARLAICharacter::OnPawnSeen);
}

void AARLAICharacter::OnPawnSeen(APawn* Pawn)
{
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
		BBComp->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}



