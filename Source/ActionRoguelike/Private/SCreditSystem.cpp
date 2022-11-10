// Fill out your copyright notice in the Description page of Project Settings.


#include "SCreditSystem.h"

void ASCreditSystem::AddCredits(int32 Delta)
{
	if (Delta < 0)
	{
		return;
	}

	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool ASCreditSystem::RemoveCredits(int32 Delta)
{
	if (Delta < 0)
	{
		return false;
	}

	if (Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);

	return true;
}

int32 ASCreditSystem::GetCredits() const
{
	return Credits;
}
