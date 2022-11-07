// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/SAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "SCharacter.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimationDelay = 0.2f;
	LineTraceLength = 5000.0f;
}

void USAction_ProjectileAttack::Start_Implementation(AActor* Instigator)
{
	Super::Start_Implementation(Instigator);

	ASCharacter* Character = Cast<ASCharacter>(Instigator);

	if (Character)
	{
		Character->PlayAnimMontage(AttackAnimation);

		if (ProjectileSpawnVFX != nullptr)
			UGameplayStatics::SpawnEmitterAttached(ProjectileSpawnVFX, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
	
		FTimerHandle TimerHandleAttackDelay;
		FTimerDelegate TimerDelegate;

		TimerDelegate.BindUFunction(this, "AttackDelayElapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandleAttackDelay, TimerDelegate, AttackAnimationDelay, false);
	}
}

void USAction_ProjectileAttack::AttackDelayElapsed(ASCharacter* InstigatorCharacter)
{
	if (ProjectileClass == nullptr)
		return;

	OnAttack(InstigatorCharacter);
}

void USAction_ProjectileAttack::OnAttack(ASCharacter* InstigatorCharacter)
{
	FVector socketLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

	FHitResult hitResult;
	FVector endLocation;

	if (IsLineTraceHit(InstigatorCharacter, hitResult, endLocation))
		endLocation = hitResult.ImpactPoint;

	SpawnProjectile(InstigatorCharacter, ProjectileClass, socketLocation, endLocation);
}

bool USAction_ProjectileAttack::IsLineTraceHit(ASCharacter* InstigatorCharacter, FHitResult& outHitResult, FVector& outEndLocation)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(InstigatorCharacter);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

	FVector startLocation = InstigatorCharacter->GetPawnViewLocation();
	outEndLocation = startLocation + (InstigatorCharacter->GetControlRotation().Vector() * LineTraceLength);

	return GetWorld()->LineTraceSingleByObjectType(outHitResult, startLocation, outEndLocation, ObjectParams, Params);
}

void USAction_ProjectileAttack::SpawnProjectile(ASCharacter* InstigatorCharacter, TSubclassOf<AActor>& projectile, FVector& startLocation, const FVector& endLocation)
{
	FRotator projectileRotation = (endLocation - startLocation).Rotation();
	FTransform SpawnMatrix = FTransform(projectileRotation, startLocation);
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParameters.Instigator = InstigatorCharacter;

	GetWorld()->SpawnActor<AActor>(projectile, SpawnMatrix, SpawnParameters);

	Stop(InstigatorCharacter);
}

