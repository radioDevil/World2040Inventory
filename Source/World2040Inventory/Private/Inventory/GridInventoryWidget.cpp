// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/GridInventoryWidget.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Inventory/BaseInventoryComponent.h"
#include "Inventory/ItemObject.h"
#include "Inventory/ItemSlotWidget.h"

void UGridInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

 	InventoryComponent = Cast<UBaseInventoryComponent>(GetOwningPlayerPawn()->GetComponentByClass(UBaseInventoryComponent::StaticClass()));
}

bool UGridInventoryWidget::Initialize()
{
	bool Result = Super::Initialize();

	if (!GridBorder) return Result;
	
	FVector2D NewSize;
	NewSize.X = Rows * TileSize;
	NewSize.Y = Column * TileSize;
	
	UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder)->SetSize(NewSize);
	
	CreateLineSegment();

	return Result;
}

void UGridInventoryWidget::Draw()
{
	if (!GridBorder) return;
	
	for (size_t i = 0; i < Lines.Num(); ++i)
	{
		FPaintContext Context;
	
		FVector2D LocalTopLeft = USlateBlueprintLibrary::GetLocalTopLeft(GridBorder->GetCachedGeometry());
		
		UWidgetBlueprintLibrary::DrawLine(Context, LocalTopLeft + Lines[i].StartXY, LocalTopLeft + Lines[i].EndXY);
	}
}

void UGridInventoryWidget::CreateLineSegment()
{
	for (int32 i = 0; i < Column; i++)
	{
		const float MagnitudeX = i * TileSize;
	
		FGridOffset TempOffset;
		TempOffset.StartXY = FVector2d(MagnitudeX, 0);
		TempOffset.EndXY = FVector2d(MagnitudeX, Rows * TileSize);
		
		Lines.Add(TempOffset);
	}
	
	for (int32 i = 0; i < Rows; i++)
	{
		const float MagnitudeY = i * TileSize;
	
		FGridOffset TempOffset;
		TempOffset.StartXY = FVector2d(0, MagnitudeY);
		TempOffset.EndXY = FVector2d(Column * TileSize, MagnitudeY);
		
		Lines.Add(TempOffset);
	}
}

void UGridInventoryWidget::RefreshInventorySlots()
{
	if (!GridCanvas)
	{
		UE_LOG(LogTemp, Display, TEXT("bad GridCanvas"));
	}

	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("bad InventoryComponent"));
	}

	if (!SlotWidgetClass)
	{
		UE_LOG(LogTemp, Display, TEXT("bad SlotWidgetClass"));
	}

	if (!GridCanvas || !InventoryComponent || !SlotWidgetClass) return;
	
	for (UWidget* Child : GridCanvas->GetAllChildren())
	{
		if (GridCanvas->GetChildIndex(Child) == 0) continue;
		
		GridCanvas->RemoveChild(Child);
	}
	
	size_t count = 0;

	TArray<FItemInfo> ItemsArray = InventoryComponent->GetEntries();
	
	for (const FItemInfo& ItemInfo : ItemsArray)
	{
		UItemSlotWidget* Widget = CreateWidget<UItemSlotWidget>(this, SlotWidgetClass);
		if (!Widget) continue;

		Widget->ItemInfo = ItemInfo;
		
		if (IsValid(ItemInfo.Image))
		{
			Widget->Image->SetBrushFromTexture(ItemInfo.Image);
		}
		
		GridCanvas->AddChild(Widget);
		Widget->SlotRefresh(100, ItemInfo.Size.X, ItemInfo.Size.Y);
	
		if (UCanvasPanelSlot* PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Widget))
		{
			PanelSlot->SetAutoSize(true);
			PanelSlot->SetPosition(FVector2D(ItemInfo.Tiling.X * TileSize, ItemInfo.Tiling.Y * TileSize));
		}
		
		count++;
	}
}
