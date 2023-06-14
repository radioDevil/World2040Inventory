// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GridInventoryWidget.generated.h"

class UCanvasPanel;
class UBorder;
class UBaseInventoryComponent;
class UItemSlotWidget;

USTRUCT(BlueprintType)
struct FGridOffset
{
	GENERATED_BODY()

	FVector2D StartXY;
	FVector2D EndXY;
};

UCLASS(Abstract)
class WORLD2040INVENTORY_API UGridInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetInventoryID(int32 NewID) { InventoryID = NewID; };

	UFUNCTION(BlueprintCallable)
	void Draw();

	UFUNCTION(BlueprintCallable)
	void CreateLineSegment();

	UFUNCTION(BlueprintCallable)
	void RefreshInventorySlots();
	
protected:
	UPROPERTY(BlueprintReadOnly)
	UBaseInventoryComponent* InventoryComponent;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemSlotWidget> SlotWidgetClass;
	
	UPROPERTY(EditAnywhere)
	float TileSize = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 InventoryID = 0;

	UPROPERTY(EditAnywhere)
	int32 Column = 5;
	
	UPROPERTY(EditAnywhere)
	int32 Rows = 5;

	UPROPERTY(EditAnywhere, meta = (BindWidget), BlueprintReadOnly)
	UCanvasPanel* GridCanvas;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget), BlueprintReadOnly)
	UBorder* GridBorder;

protected:
	TArray<FGridOffset> Lines;
};
