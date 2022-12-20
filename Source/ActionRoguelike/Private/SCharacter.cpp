// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "..\Public\SCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SActionComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));
	SpringArmComponent->bUsePawnControlRotation = bUsePawnControlRotation;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>(TEXT("Interaction Component"));
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attribute Component"));
	ActionComponent = CreateDefaultSubobject<USActionComponent>(TEXT("Action Component"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void ASCharacter::HealSelf(float amount /* = 100 */)
{
	AttributeComponent->ApplyHealthChange(this, amount);
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(HorizontalAxisName, this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis(VerticalAxisName, this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis(YawAxisName, this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(PitchAxisName, this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction(PrimaryAttackName, IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction(PrimaryBlackholeName, IE_Pressed, this, &ASCharacter::PrimaryBlackhole);
	PlayerInputComponent->BindAction(PrimaryDashName, IE_Pressed, this, &ASCharacter::PrimaryDash);
	PlayerInputComponent->BindAction(PrimaryInteractName, IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction(JumpKeyName, IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(SprintKeyName, IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction(SprintKeyName, IE_Released, this, &ASCharacter::SprintStop);
}

void ASCharacter::MoveForward(float value)
{
	FRotator controlRotator = GetControlRotation();
	controlRotator.Pitch = 0.0f;
	controlRotator.Roll = 0.0f;

	AddMovementInput(controlRotator.Vector(), value);
}

void ASCharacter::MoveRight(float value)
{
	FRotator controlRotator = GetControlRotation();
	controlRotator.Pitch = 0.0f;
	controlRotator.Roll = 0.0f;

	FVector rightVector = FRotationMatrix(controlRotator).GetScaledAxis(EAxis::Y);

	AddMovementInput(rightVector, value);
}

void ASCharacter::SprintStart()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void ASCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, "ProjectileAttack");
}

void ASCharacter::PrimaryBlackhole()
{
	ActionComponent->StartActionByName(this, "Blackhole");
}

void ASCharacter::PrimaryDash()
{
	ActionComponent->StartActionByName(this, "Dash");
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComponent == nullptr)
		return;

	InteractionComponent->PrimaryInteract();
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{

	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());

		if (playerController != nullptr)
			DisableInput(playerController);

		return;
	}

	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(FlashMaterialName, GetWorld()->TimeSeconds);
		float RageDelta = FMath::Abs(Delta);

		AttributeComponent->ApplyRage(InstigatorActor, RageDelta);
	}
}

