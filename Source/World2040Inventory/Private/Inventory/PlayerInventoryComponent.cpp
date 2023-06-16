// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/PlayerInventoryComponent.h"

#include "Inventory/ItemObject.h"
#include "Net/UnrealNetwork.h"

void UPlayerInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, WeaponInfo);
}

void UPlayerInventoryComponent::TryChangeEquipmentWeapon(FItemInfo Info)
{
	ChangeEquipmentWeapon(Info);
	if (!GetOwner()->HasAuthority())
	{
		ChangeEquipmentWeapon_Server(Info);
	}
}

void UPlayerInventoryComponent::ChangeEquipmentWeapon(FItemInfo Info)
{
	WeaponInfo = Info;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Yellow, TEXT("Change equipment"));
	}
}

void UPlayerInventoryComponent::ChangeEquipmentWeapon_Server_Implementation(FItemInfo Info)
{
	ChangeEquipmentWeapon(Info);
}