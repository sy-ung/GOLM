// Fill out your copyright notice in the Description page of Project Settings.

#include "GOLM.h"
#include "GOLMPlayerState.h"


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

EGetWeapon AGOLMPlayerState::GetWeaponFor(EEquipSlot slot)
{
	switch (slot)
	{
	case EEquipSlot::HAND_SLOT:
		return HandWeapon;
	default:
		break;
	}
	return EGetWeapon::NONE;
}

void AGOLMPlayerState::SetWeaponFor(EGetWeapon Weapon, EEquipSlot Slot)
{
	switch (Slot)
	{
	case EEquipSlot::HAND_SLOT:
		HandWeapon = Weapon;
		break;
	default:
		break;
	}
}