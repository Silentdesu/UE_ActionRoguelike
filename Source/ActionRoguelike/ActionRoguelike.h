// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreen(UObject* WorldContext, FString Message, FColor Color = FColor::White, float Duration = 5.0f)
{
	if (!WorldContext)
	{
		return;
	}

	UWorld* World = WorldContext->GetWorld();

	if (!World)
	{
		return;
	}

	FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT]" : "[SERVER]";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Message);
	}
}
