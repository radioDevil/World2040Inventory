// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSlotWidget.generated.h"

class UOverlay;
class UItemSlotWidget;

UCLASS()
class WORLD2040INVENTORY_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation) override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget), BlueprintReadOnly)
	UOverlay* MainOverlay;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemSlotWidget> SlotWidgetClass;

	UPROPERTY()
	UItemSlotWidget* CurrentEquipmentSlotWidget;
};
