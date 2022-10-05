// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

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

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName HorizontalAxisName = "Horizontal";

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName VerticalAxisName = "Vertical";

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName YawAxisName = "Turn";

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName PitchAxisName = "LookUp";

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName PrimaryAttackName = "Primary Attack";

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName JumpKeyName = "Jump";

	UPROPERTY(EditAnywhere, Category = "Sockets")
	FName EffectSocketName = "Muzzle_01";

	UPROPERTY(EditAnywhere, Category = "Basic settings")
	bool bUsePawnControlRotation = true;

	UPROPERTY(EditAnywhere, Category = "Other classes")
	TSubclassOf<AActor> ProjectileClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void PrimaryAttack();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
