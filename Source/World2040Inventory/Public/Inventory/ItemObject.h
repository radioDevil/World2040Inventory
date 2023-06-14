// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInventoryComponent.h"
#include "UObject/NoExportTypes.h"
#include "ItemObject.generated.h"


UCLASS(Blueprintable, BlueprintType)
class WORLD2040INVENTORY_API UItemObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemInfo ItemInfo;
};
