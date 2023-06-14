// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, AActor*, InteractInstigator);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORLD2040INVENTORY_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteract OnInteract;
};
