// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Kismet/GameplayStatics.h"

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASDashProjectile::Explode, ExplodeDelay);
}

void ASDashProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);

	UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeEmitter, GetActorLocation(), GetActorRotation());
	SetActorEnableCollision(false);

	FTimerHandle teleportDelayHandle;

	GetWorldTimerManager().SetTimer(teleportDelayHandle, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);
}

void ASDashProjectile::TeleportInstigator()
{
	AActor* instigator = GetInstigator();

	if (ensure(instigator))
	{
		instigator->TeleportTo(GetActorLocation(), instigator->GetActorRotation());
	}

	Destroy();
}

