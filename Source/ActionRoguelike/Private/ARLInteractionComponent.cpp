// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLInteractionComponent.h"
#include "ARLGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "ProfilingDebugging/CookStats.h"


static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("arl.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

// Sets default values for this component's properties
UARLInteractionComponent::UARLInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UARLInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UARLInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UARLInteractionComponent::PrimaryInteract()	// @fixme: Make Interaction trace hit where crosshair is pointed
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + (EyeRotation.Vector() * 200);

	float Radius = 30.f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	
	TArray<FHitResult> Hits;
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (bDebugDraw)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
			}
			
			if (HitActor->Implements<UARLGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
			
				IARLGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
	}
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	}
	
}
