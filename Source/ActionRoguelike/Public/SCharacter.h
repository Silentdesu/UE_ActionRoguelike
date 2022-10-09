// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName HorizontalAxisName = "Horizontal";

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName VerticalAxisName = "Vertical";

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName YawAxisName = "Turn";

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName PitchAxisName = "LookUp";

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName PrimaryAttackName = "PrimaryAttack";

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName PrimaryBlackholeName = "PrimaryBlackhole";

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName PrimaryDashName = "PrimaryDash";

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName PrimaryInteractName = "PrimaryInteractName";

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName JumpKeyName = "Jump";

	UPROPERTY(EditAnywhere, Category = "Sockets")
	FName EffectSocketName = "Muzzle_01";

	UPROPERTY(EditAnywhere, Category = "Basic settings")
	bool bUsePawnControlRotation = true;

	UPROPERTY(EditAnywhere, Category = "Line trace settings")
	float LineTraceLength = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<AActor> BlackholeProjectileClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void PrimaryAttack();
	void PrimaryInteract();
	void PrimaryDash();
	void PrimaryBlackhole();

	bool IsLineTraceHit(FHitResult& outHitResult, FVector& outEndLocation);
	void SpawnProjectile(TSubclassOf<AActor>& projectile, FVector& startLocation, const FVector& endLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
