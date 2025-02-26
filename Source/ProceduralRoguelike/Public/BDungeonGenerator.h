// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BDungeonGenerator.generated.h"

class ABDungeonRoom;
class UArrowComponent;

UCLASS()
class PROCEDURALROGUELIKE_API ABDungeonGenerator : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABDungeonGenerator();

protected:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ABDungeonRoom> SpawnRoomClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ABDungeonRoom> BossRoomClass;

    UPROPERTY(EditDefaultsOnly)
    TArray<USceneComponent*> SpawnDirectionList;

    UPROPERTY(EditDefaultsOnly)
    TArray<UPrimitiveComponent*> OverlappedList;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<ABDungeonRoom> LatestRoom;

    UPROPERTY(EditAnywhere)
    int32 RoomCount;

    void SpawnStartRoom();

    void SpawnNextRoom();

    void CheckOverlappedRooms();

    ABDungeonRoom* SpawnRoom(UClass* SpawnClass, const FTransform& Transform);
    
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
