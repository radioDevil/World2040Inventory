// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/EquipmentSlotWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Inventory/ItemSlotWidget.h"
#include "Inventory/PlayerInventoryComponent.h"
#include "Inventory/SlotDragDropOperation.h"

bool UEquipmentSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                        UDragDropOperation* InOperation)
{
	// TODO: FIX IT, TILE IT SHOULD BE INVENTORY COMPONENT
	const float TileSize = 100.f;
	
	const bool Result = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (CurrentEquipmentSlotWidget)
	{
		CurrentEquipmentSlotWidget->RemoveFromParent();
	}
	
	if (!SlotWidgetClass || !MainOverlay)
	{
		return Result;
	}

	USlotDragDropOperation* SlotOperation = Cast<USlotDragDropOperation>(InOperation);
	if (!SlotOperation || !SlotOperation->ItemSlotWidget)
	{
		return Result;
	}

	const FItemInfo ItemInfo = SlotOperation->ItemSlotWidget->ItemInfo;
	
	UItemSlotWidget* Widget = CreateWidget<UItemSlotWidget>(this, SlotWidgetClass);
	if (!Widget)
	{
		return Result;
	}

	CurrentEquipmentSlotWidget = Widget;

	Widget->ItemInfo = ItemInfo;
	
	if (IsValid(ItemInfo.Image))
	{
		Widget->Image->SetBrushFromTexture(ItemInfo.Image);
	}

	MainOverlay->AddChild(Widget);
	Widget->SlotRefresh(TileSize, ItemInfo.Size.X, ItemInfo.Size.Y);

	if (UCanvasPanelSlot* PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Widget))
	{
		PanelSlot->SetAutoSize(true);
		PanelSlot->SetPosition(FVector2D(ItemInfo.Size.X * TileSize, ItemInfo.Size.Y * TileSize));
		PanelSlot->SetAlignment(FVector2D(2.f, 2.f));
	}

	UPlayerInventoryComponent* PlayerInventoryComponent = Cast<UPlayerInventoryComponent>(
														GetOwningPlayerPawn()->GetComponentByClass(UPlayerInventoryComponent::StaticClass()));

	if (PlayerInventoryComponent)
	{
		PlayerInventoryComponent->TryChangeEquipmentWeapon(ItemInfo);
	}
	
	return Result;
}
