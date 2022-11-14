// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USBaseWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

public:

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceLineLength;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float SweepSphereRadius;

	UPROPERTY(EditAnywhere, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

public:

	void PrimaryInteract();
	void FindBestInteractable();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USBaseWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	USBaseWorldUserWidget* DefaultWidgetInstance;

	UPROPERTY()
	AActor* FocusedActor;
		
};
