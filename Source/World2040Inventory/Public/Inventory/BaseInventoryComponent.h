// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseInventoryComponent.generated.h"

class UItemObject;

USTRUCT(BlueprintType)
struct FPoint2D
{
	GENERATED_BODY()

	FPoint2D()
	{
		X = 0;
		Y = 0;
	}
    
	FPoint2D(const int32 InX, const int32 InY)
	{
		X = InX;
		Y = InY;
	}
    
	FPoint2D(const FPoint2D& Other)
	{
		X = Other.X;
		Y = Other.Y;
	}
	    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UniqueID = 0;
	
	bool operator==(const FPoint2D& Other) const
	{
		return (X == Other.X && Y == Other.Y);
	}
    
	FPoint2D operator+(const FPoint2D& Other) const
	{
		return FPoint2D(X + Other.X, Y + Other.Y);
	}
};

USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxAmount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name = "Item Name";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ItemID = 0;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentAmount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPoint2D Size;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 UniqueID = 0;
	
	UPROPERTY(VisibleAnywhere) // !!!! Warning, maybe delete or replace  !!!!
	UItemObject* ItemInstance;

	UPROPERTY(VisibleAnywhere)
	FPoint2D Tiling;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORLD2040INVENTORY_API UBaseInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddItem(TSubclassOf<UItemObject> ItemClass);

	UFUNCTION(BlueprintCallable)
	TArray<FItemInfo> GetEntries() {return Entries;};

	UFUNCTION(BlueprintCallable)
	bool GetFreeCellsByTiling(const FPoint2D& StartTiling, FPoint2D SizeItem);

	UFUNCTION(BlueprintCallable)
	void ReplaceItemCells(FItemInfo ReplacedItem, FPoint2D NewTile);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void ReplaceItemCells_Server(FItemInfo ReplacedItem, FPoint2D NewTile);
	
protected:
	void CellsInitialize();

	void FillToCells(FItemInfo& Item);
	bool GetFreeCellsFromItem(FItemInfo Item);

	void ChangeCells(FItemInfo ReplacedItem, FPoint2D ClearTile, int32 ID, int32 UniqueID = 0);
	
	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<FItemInfo> Entries;

	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<FPoint2D> Cells;

	UPROPERTY(EditAnywhere)
	int32 Columns = 5;

	UPROPERTY(EditAnywhere)
	int32 Rows = 5;
};
