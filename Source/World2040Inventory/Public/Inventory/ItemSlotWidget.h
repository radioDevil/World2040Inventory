// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

class USizeBox;
class UImage;
class UItemBase;

UCLASS()
class WORLD2040INVENTORY_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SlotRefresh(float TitleSize = 100, int32 DimensionX = 1, int32 DimensionY = 1);

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UImage* Image;

	UPROPERTY(BlueprintReadOnly)
	FItemInfo ItemInfo;
	
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	USizeBox* Box;
};
