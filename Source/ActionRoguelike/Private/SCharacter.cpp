// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "..\Public\SCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"

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

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("Interaction Comp"));

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
	PlayerInputComponent->BindAction(PrimaryBlackholeName, IE_Pressed, this, &ASCharacter::PrimaryBlackhole);
	PlayerInputComponent->BindAction(PrimaryDashName, IE_Pressed, this, &ASCharacter::PrimaryDash);
	PlayerInputComponent->BindAction(PrimaryInteractName, IE_Pressed, this, &ASCharacter::PrimaryInteract);
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
	if (!ensure(ProjectileClass))
		return;

	FVector socketLocation = GetMesh()->GetSocketLocation(EffectSocketName);

	FHitResult hitResult;
	FVector endLocation;

	if (IsLineTraceHit(hitResult, endLocation))
		endLocation = hitResult.ImpactPoint;

	SpawnProjectile(ProjectileClass, socketLocation, endLocation);
}

void ASCharacter::PrimaryBlackhole()
{
	if (!ensure(BlackholeProjectileClass))
		return;

	FVector socketLocation = GetMesh()->GetSocketLocation(EffectSocketName);
	
	FHitResult hitResult;
	FVector endLocation;

	if (IsLineTraceHit(hitResult, endLocation))
		endLocation = hitResult.ImpactPoint;

	SpawnProjectile(BlackholeProjectileClass, socketLocation, endLocation);
}

void ASCharacter::PrimaryDash()
{
	if (!ensure(DashProjectileClass))
		return;

	FVector socketLocation = GetMesh()->GetSocketLocation(EffectSocketName);
	
	FHitResult hitResult;
	FVector endLocation;

	if (IsLineTraceHit(hitResult, endLocation))
		endLocation = hitResult.ImpactPoint;

	SpawnProjectile(DashProjectileClass, socketLocation, endLocation);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp == nullptr)
		return;

	InteractionComp->PrimaryInteract();
}

bool ASCharacter::IsLineTraceHit(FHitResult& outHitResult, FVector& outEndLocation)
{
	FCollisionObjectQueryParams collisionQueryParams;
	collisionQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	collisionQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FVector startLocation = CameraComp->GetComponentLocation();
	outEndLocation = startLocation + (CameraComp->GetComponentRotation().Vector() * LineTraceLength);

	return GetWorld()->LineTraceSingleByObjectType(outHitResult, startLocation, outEndLocation, collisionQueryParams);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor>& projectile, FVector& startLocation, const FVector& endLocation)
{
	FRotator projectileRotation = (endLocation - startLocation).Rotation();
	FTransform SpawnMatrix = FTransform(projectileRotation, startLocation);
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParameters.Instigator = this;

	GetWorld()->SpawnActor<AActor>(projectile, SpawnMatrix, SpawnParameters);
}

