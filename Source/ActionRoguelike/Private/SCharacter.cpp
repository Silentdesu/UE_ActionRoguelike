// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "..\Public\SCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = bUsePawnControlRotation;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	PlayerInputComponent->BindAction(JumpKeyName, IE_Pressed, this, &ACharacter::Jump);
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

void ASCharacter::PrimaryAttack()
{
	FVector socketLocation = GetMesh()->GetSocketLocation(EffectSocketName);

	FTransform SpawnMatrix = FTransform(GetControlRotation(), socketLocation);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnMatrix, SpawnParameters);
}

