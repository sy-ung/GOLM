// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMPlayerState.h"
#include "Weapon.h"


void AGOLMPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AGOLMPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AGOLMPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGOLMPlayerState, PlayerKills);
	DOREPLIFETIME(AGOLMPlayerState, PlayerDeath);

	DOREPLIFETIME(AGOLMPlayerState, HandWeapon);
	DOREPLIFETIME(AGOLMPlayerState, RightShoulder);
	DOREPLIFETIME(AGOLMPlayerState, LeftShoulder);

	DOREPLIFETIME(AGOLMPlayerState, bIsKyle);

}

void AGOLMPlayerState::AddToScore(int32 AdditionalKill, int32 AdditionalDeath)
{
	if (Role == ROLE_Authority)
	{
		PlayerKills += AdditionalKill;
		PlayerDeath += AdditionalDeath;
	}
}


AWeapon *AGOLMPlayerState::GetWeaponFor(EEquipSlot slot)
{
	switch (slot)
	{
	case EEquipSlot::HAND_SLOT:
		if(HandWeapon!=NULL)
		{
			return HandWeapon;
		}
		break;
	case EEquipSlot::LEFT_SHOULDER:
		if(LeftShoulder != NULL)
			return LeftShoulder;
		break;
	case EEquipSlot::RIGHT_SHOULDER:
		if (RightShoulder != NULL)
			return RightShoulder;
		break;
	default:
		break;
	}
	return NULL;
}

void AGOLMPlayerState::SetWeaponFor(AWeapon* Weapon, EEquipSlot Slot)
{
	if(Role == ROLE_Authority)
	{
		switch (Slot)
		{
		case EEquipSlot::HAND_SLOT:
			HandWeapon = Weapon;
			break;
		case EEquipSlot::LEFT_SHOULDER:
			LeftShoulder = Weapon;
			break;
		case EEquipSlot::RIGHT_SHOULDER:
			RightShoulder = Weapon;
			break;
		default:
			break;
		}
	}
	else
	{
		ServerSetWeaponFor(Weapon, Slot);
	}
}

void AGOLMPlayerState::ServerSetWeaponFor_Implementation(AWeapon* Weapon, EEquipSlot Slot)
{
	SetWeaponFor(Weapon, Slot);
}
bool AGOLMPlayerState::ServerSetWeaponFor_Validate(AWeapon* Weapon, EEquipSlot Slot)
{
	return true;
}