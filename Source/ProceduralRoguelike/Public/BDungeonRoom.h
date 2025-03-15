// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BDungeonRoom.generated.h"

class UBoxComponent;

UCLASS()
class PROCEDURALROGUELIKE_API ABDungeonRoom : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UBoxComponent> BoxComponent;
    
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> ExitComponent;
    
    // Sets default values for this actor's properties
    ABDungeonRoom();
};
