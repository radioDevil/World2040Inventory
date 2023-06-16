// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SlotDragDropOperation.generated.h"

class UItemSlotWidget;

UCLASS()
class WORLD2040INVENTORY_API USlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drag and Drop", meta=( ExposeOnSpawn="true" ))
	UItemSlotWidget* ItemSlotWidget;
};
