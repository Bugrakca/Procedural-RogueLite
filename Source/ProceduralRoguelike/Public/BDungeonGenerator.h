// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BDungeonGenerator.generated.h"

class UBRandomSeedComponent;
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
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UBRandomSeedComponent> RandomSeedComp;
    
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ABDungeonRoom> SpawnRoomClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ABDungeonRoom> BossRoomClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ABDungeonRoom> RegularRoomClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> SpawnWallClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> SpawnDoorClass;

    UPROPERTY(EditDefaultsOnly)
    TArray<USceneComponent*> SpawnDirectionList;

    UPROPERTY(EditDefaultsOnly)
    TArray<USceneComponent*> DoorDirectionList;

    UPROPERTY(EditDefaultsOnly)
    TArray<TSubclassOf<ABDungeonRoom>> RoomList;

    UPROPERTY(EditAnywhere)
    TArray<UPrimitiveComponent*> OverlappedList;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<ABDungeonRoom> LatestRoom;

    UPROPERTY(VisibleAnywhere)
    int32 RoomCount;

    UPROPERTY(EditAnywhere)
    int32 MaxRoomNumber;

    UPROPERTY(EditDefaultsOnly)
    bool bIsDungeonComplete;

    void SpawnStartRoom();

    void SpawnNextRoom();

    void FinalizeDungeon();

    void CheckOverlappedRooms();

    ABDungeonRoom* RoomSpawn(UClass* SpawnClass, const FTransform& Transform);

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
