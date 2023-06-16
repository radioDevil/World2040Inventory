// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/BaseInventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORLD2040INVENTORY_API UPlayerInventoryComponent : public UBaseInventoryComponent
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void TryChangeEquipmentWeapon(FItemInfo Info);

	
protected:
	UFUNCTION()
	void ChangeEquipmentWeapon(FItemInfo Info);
	
	UFUNCTION(Server, Unreliable)
	void ChangeEquipmentWeapon_Server(FItemInfo Info);
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	FItemInfo WeaponInfo;
};
