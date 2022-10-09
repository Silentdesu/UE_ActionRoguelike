// Fill out your copyright notice in the Description page of Project Settings.


#include "SBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/SparseDelegate.h"

// Sets default values
ASBarrel::ASBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RootComponent = StaticMeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
	
	RadialForceComponent->SetAutoActivate(false);
	RadialForceComponent->Radius = 750.0f;
	RadialForceComponent->ForceStrength = 2500.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void ASBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ASBarrel::OnActorHit);
}

// Called every frame
void ASBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("ActorHit is Explosive Barrel!"));
}

