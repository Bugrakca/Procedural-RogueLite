// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BInputConfigData.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class PROCEDURALROGUELIKE_API UBInputConfigData : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> IAction_Move;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> IAction_Look;
};
