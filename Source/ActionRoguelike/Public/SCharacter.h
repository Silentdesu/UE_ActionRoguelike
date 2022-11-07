// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class USAttributeComponent;
class USActionComponent;
class UProjectileSystem;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

public:

	UFUNCTION(Exec)
	void HealSelf(float amount = 100);

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

protected:

	virtual void PostInitializeComponents() override;

protected:

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

protected:

	void MoveForward(float value);
	void MoveRight(float value);
	void SprintStart();
	void SprintStop();
	void PrimaryAttack();
	void PrimaryInteract();
	void PrimaryDash();
	void PrimaryBlackhole();

protected:
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComponent;

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

	UPROPERTY(EditAnywhere, Category = "Input settings")
	FName SprintKeyName = "Sprint";

	UPROPERTY(EditAnywhere, Category = "Material settings")
	FName FlashMaterialName = "TimeToHit";

	UPROPERTY(EditAnywhere, Category = "Basic settings")
	bool bUsePawnControlRotation = true;
};
