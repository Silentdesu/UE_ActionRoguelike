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

// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = bUsePawnControlRotation;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("Interaction Comp"));
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("Attribute Comp"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void ASCharacter::HealSelf(float amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, amount);
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
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
	if (ProjectileClass == nullptr)
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
	if (BlackholeProjectileClass == nullptr)
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
	if (DashProjectileClass == nullptr)
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

	if (ProjectileSpawnVFX != nullptr)
		UGameplayStatics::SpawnEmitterAttached(ProjectileSpawnVFX, GetMesh(), EffectSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

	GetWorld()->SpawnActor<AActor>(projectile, SpawnMatrix, SpawnParameters);
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{

	if (NewHealth <= 0.f && Delta < 0.f)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());

		if (playerController != nullptr)
			DisableInput(playerController);

		return;
	}

	GetMesh()->SetScalarParameterValueOnMaterials(FlashMaterialName, GetWorld()->TimeSeconds);
}

