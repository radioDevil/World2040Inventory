// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/BaseInventoryComponent.h"
#include "Inventory/ItemObject.h"
#include "Net/UnrealNetwork.h"

UBaseInventoryComponent::UBaseInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
}

void UBaseInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Entries);
	DOREPLIFETIME(ThisClass, Cells);
}

void UBaseInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	CellsInitialize();
}

void UBaseInventoryComponent::AddItem(TSubclassOf<UItemObject> ItemClass)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	UItemObject* ItemInstance = NewObject<UItemObject>(GetOwner(), ItemClass);
	if (!ItemInstance)
	{
		return;
	}

	 if (!GetFreeCellsFromItem(ItemInstance->ItemInfo))
	 {
	 	return;
	 }

	for (FItemInfo& Item : Entries)
	{
		if (Item.ItemID == ItemInstance->ItemInfo.ItemID)
		{
			if (Item.CurrentAmount >= Item.MaxAmount)
			{
				break;
			}
			
			Item.CurrentAmount += ItemInstance->ItemInfo.CurrentAmount;
		}
	}

	ItemInstance->ItemInfo.UniqueID = ItemInstance->GetUniqueID();
	FillToCells(ItemInstance->ItemInfo);
	Entries.Add(ItemInstance->ItemInfo);
}

bool UBaseInventoryComponent::GetFreeCellsByTiling(const FPoint2D& StartTiling, FPoint2D SizeItem)
{
	bool Result = false;
	
	const int32 IndentX = StartTiling.X + (SizeItem.X);
	const int32 IndentY = StartTiling.Y + (SizeItem.Y);

	int32 Decrease = (SizeItem.X + SizeItem.Y);

	if (SizeItem.X == 1 && SizeItem.Y == 1)
	{
		Decrease = 1;
	}
	else if ((SizeItem.X > 1 && SizeItem.Y > 1) && SizeItem.X != SizeItem.Y)
	{
		Decrease += 1;
	}
	else if (SizeItem.X == 1 || SizeItem.Y == 1)
	{
		Decrease -= 1;
	}
	
	for (int32 y = StartTiling.Y; y < IndentY; y++)
	{
		for (int32 x = StartTiling.X; x < IndentX; x++)
		{
			int32 TempIndex = -1;
			Cells.Find(FPoint2D(x, y), TempIndex);

			if (Cells.IsValidIndex(TempIndex) && (Cells[TempIndex].ItemID <= 0 || Cells[TempIndex].UniqueID == StartTiling.UniqueID))
			{
				Decrease--;
			}

			if (Decrease <= 0)
			{
				break;
			}
		}
	}

	if (Decrease <= 0)
	{
		Result = true;
	}

	return Result;
}

void UBaseInventoryComponent::ReplaceItemCells(FItemInfo ReplacedItem, FPoint2D NewTile)
{
	NewTile.X = FMath::Clamp(NewTile.X, 0, Rows - 1);
	NewTile.Y = FMath::Clamp(NewTile.Y, 0, Columns - 1);
	NewTile.ItemID = ReplacedItem.ItemID;
	NewTile.UniqueID = ReplacedItem.UniqueID;

	if (!GetFreeCellsByTiling(NewTile, ReplacedItem.Size)) return;

	ChangeCells(ReplacedItem, ReplacedItem.Tiling, -1);
	ChangeCells(ReplacedItem, NewTile, ReplacedItem.ItemID, ReplacedItem.UniqueID);

	for (FItemInfo& Item : Entries)
	{
		if (Item.Tiling.X != ReplacedItem.Tiling.X || Item.Tiling.Y != ReplacedItem.Tiling.Y)
		{
			continue;
		}

		if (Item.ItemID == ReplacedItem.ItemID)
		{
			Item.Tiling = NewTile;
		}
	}
}

void UBaseInventoryComponent::ReplaceItemCells_Server_Implementation(FItemInfo ReplacedItem, FPoint2D NewTile)
{
	NewTile.X = FMath::Clamp(NewTile.X, 0, Rows - 1);
	NewTile.Y = FMath::Clamp(NewTile.Y, 0, Columns - 1);
	NewTile.ItemID = ReplacedItem.ItemID;
	NewTile.UniqueID = ReplacedItem.UniqueID;

	if (!GetFreeCellsByTiling(NewTile, ReplacedItem.Size)) return;

	ChangeCells(ReplacedItem, ReplacedItem.Tiling, -1);
	ChangeCells(ReplacedItem, NewTile, ReplacedItem.ItemID, ReplacedItem.UniqueID);

	for (FItemInfo& Item : Entries)
	{
		if (Item.Tiling.X != ReplacedItem.Tiling.X || Item.Tiling.Y != ReplacedItem.Tiling.Y)
		{
			continue;
		}

		if (Item.ItemID == ReplacedItem.ItemID)
		{
			Item.Tiling = NewTile;
		}
	}
}

void UBaseInventoryComponent::CellsInitialize()
{
	Cells.Empty();

	for (int32 X = 0; X != Rows; X++)
	{
		for (int32 Y = 0; Y != Columns; Y++)
		{
			Cells.Add(FPoint2D(Y, X)); // TODO : FIX IT ('X' - IS NOT 'Y')
		}
	}
}

void UBaseInventoryComponent::FillToCells(FItemInfo& Item)
{
	for (FPoint2D& Cell : Cells)
	{
		if (Cell.ItemID > 0)
		{
			continue;
		}
		
		int32 Decrease = (Item.Size.X + Item.Size.Y);

		if (Item.Size.X == 1 && Item.Size.Y == 1)
		{
			Decrease = 1;
		}
		else if ((Item.Size.X > 1 && Item.Size.Y > 1) && Item.Size.X != Item.Size.Y)
		{
			Decrease += 1;
		}
		else if (Item.Size.X == 1 || Item.Size.Y == 1)
		{
			Decrease -= 1;
		}
		
		const int32 IndentX = Cell.X + (Item.Size.X);
		const int32 IndentY = Cell.Y + (Item.Size.Y);
		
		for (int32 y = Cell.Y; y < IndentY; ++y)
		{
			for (int32 x = Cell.X; x < IndentX; ++x)
			{
				int32 TempIndex = -1;
				if (!Cells.Find(FPoint2D(x, y), TempIndex))
				{
					continue;
				}
	
				if (Cells.IsValidIndex(TempIndex) && Cells[TempIndex].ItemID <= 0)
				{
					Decrease--;
				}
			}
		}
	
		if (Decrease <= 0)
		{
			for (int32 y = Cell.Y; y < IndentY; ++y)
			{
				for (int32 x = Cell.X; x < IndentX; ++x)
				{
					int32 TempIndex = -1;
					Cells.Find(FPoint2D(x, y), TempIndex);
	
					if (Cells.IsValidIndex(TempIndex) && Cells[TempIndex].ItemID <= 0)
					{
						Cells[TempIndex].ItemID = Item.ItemID;
						Cells[TempIndex].UniqueID = Item.UniqueID;
					}
				}
			}
			
			Item.Tiling.X = Cell.X;
			Item.Tiling.Y = Cell.Y;
	
			break;
		}
	}
}

bool UBaseInventoryComponent::GetFreeCellsFromItem(FItemInfo Item)
{
	bool Result = false;

	for (FPoint2D& Cell : Cells)
	{
		if (Cell.ItemID > 0)
		{
			continue;
		}
		
		int32 Decrease = (Item.Size.X + Item.Size.Y);

		if (Item.Size.X == 1 && Item.Size.Y == 1)
		{
			Decrease = 1;
		}
		
		if ((Item.Size.X > 1 && Item.Size.Y > 1) && Item.Size.X != Item.Size.Y)
		{
			Decrease += 1;
		}
		else if (Item.Size.X == 1 || Item.Size.Y == 1)
		{
			Decrease -= 1;
		}
		
		const int32 IndentX = Cell.X + (Item.Size.X);
		const int32 IndentY = Cell.Y + (Item.Size.Y);
		
		for (int32 y = Cell.Y; y < IndentY; ++y)
		{
			for (int32 x = Cell.X; x < IndentX; ++x)
			{
				int32 TempIndex = -1;
				Cells.Find(FPoint2D(x, y), TempIndex);
		
				if (Cells.IsValidIndex(TempIndex) && Cells[TempIndex].ItemID <= 0)
				{
					Decrease--;
				}
		
				if (Decrease <= 0)
				{
					break;
				}
			}
		}
	
		if (Decrease <= 0)
		{
			Result = true;
			return Result;
		}
	}

	return Result;
}

void UBaseInventoryComponent::ChangeCells(FItemInfo ReplacedItem, FPoint2D ClearTile, int32 ID, int32 UniqueID)
{
	const int32 IndentX = ClearTile.X + (ReplacedItem.Size.X);
	const int32 IndentY = ClearTile.Y + (ReplacedItem.Size.Y);
		
	for (int32 y = ClearTile.Y; y < IndentY; y++)
	{
		for (int32 x = ClearTile.X; x < IndentX; x++)
		{
			int32 TempIndex = -1;
				
			if (Cells.Find(FPoint2D(x, y), TempIndex))
			{
				if (Cells.IsValidIndex(TempIndex))
				{
					Cells[TempIndex].ItemID = ID;
					Cells[TempIndex].UniqueID = UniqueID;
				}
			}
		}
	}
}
