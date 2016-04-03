// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMPlayerState.h"
#include "Weapon.h"


void AGOLMPlayerState::BeginPlay()
{
	Super::BeginPlay();
	Health = 100;
}

void AGOLMPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AGOLMPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGOLMPlayerState, Health);
}

float AGOLMPlayerState::GetCurrentHealth()
{
	return Health;
}

void AGOLMPlayerState::ResetPlayer()
{

}

AWeapon *AGOLMPlayerState::GetWeaponFor(EEquipSlot slot)
{
	switch (slot)
	{
	case EEquipSlot::HAND_SLOT:
		if(HandWeapon!=NULL)
			return HandWeapon;
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