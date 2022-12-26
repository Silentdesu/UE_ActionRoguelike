// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SBaseProjectile.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SGameplayFunctionLibrary.h"
#include "SActionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SActionEffect.h"

ASMagicProjectile::ASMagicProjectile()
{

}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USActionComponent* ActionComponent = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));

		if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComponent->Velocity = -MovementComponent->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));

			return;
		}

		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
		{
			Explode();

			if (HasAuthority() && ActionComponent && ActionEffect)
			{
				ActionComponent->AddAction(GetInstigator(), ActionEffect);
			}
		}
	}
}

