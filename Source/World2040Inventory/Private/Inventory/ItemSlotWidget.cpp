// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemSlotWidget.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"

void UItemSlotWidget::SlotRefresh(float TitleSize, int32 DimensionX, int32 DimensionY)
{
	if (!Box || !Image) return;

	const float SizeX = DimensionX * TitleSize;
	const float SizeY = DimensionY * TitleSize;
	
	Box->SetWidthOverride(SizeX);
	Box->SetHeightOverride(SizeY);

	Image->SetDesiredSizeOverride(FVector2D(SizeX, SizeY));
}
