// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralRoguelike/Public/BDungeonGenerator.h"

#include "BDungeonRoom.h"
#include "Components/BoxComponent.h"


// Sets default values
ABDungeonGenerator::ABDungeonGenerator()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    LatestRoom = nullptr;
    RoomCount = 0;
}

// Called when the game starts or when spawned

void ABDungeonGenerator::BeginPlay()
{
    Super::BeginPlay();

    SpawnStartRoom();

    SpawnNextRoom();
}

// Called every frame

void ABDungeonGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABDungeonGenerator::SpawnStartRoom()
{
    ABDungeonRoom* SpawnedRoom = SpawnRoom(SpawnRoomClass, GetTransform());
    LatestRoom = SpawnedRoom;

    SpawnedRoom->ExitComponent->GetChildrenComponents(false, SpawnDirectionList);
    UE_LOG(LogTemp, Log, TEXT("%d"), SpawnDirectionList.Num())
}

void ABDungeonGenerator::SpawnNextRoom()
{
    FVector SpawnLocation;
    const int32 Index = FMath::RandRange(0, SpawnDirectionList.Num() - 1);
    USceneComponent* RandomDirection = SpawnDirectionList[Index];

    FVector Direction = RandomDirection->GetForwardVector();

    if (LatestRoom != nullptr)
    {
        float DesiredDistance = 1500.0f;
        SpawnLocation = LatestRoom->GetActorLocation() + (Direction * DesiredDistance);
    }
    else
    {
        SpawnLocation = GetActorLocation();
    }

    FTransform SpawnTransform = FTransform(RandomDirection->GetComponentRotation(), SpawnLocation, FVector::OneVector);

    LatestRoom = SpawnRoom(BossRoomClass, SpawnTransform);
    CheckOverlappedRooms();
    SpawnDirectionList.Remove(RandomDirection);
}

ABDungeonRoom* ABDungeonGenerator::SpawnRoom(UClass* SpawnClass, const FTransform& Transform)
{
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ABDungeonRoom* SpawnRoom = GetWorld()->SpawnActor<ABDungeonRoom>(SpawnClass, Transform, Params);

    return SpawnRoom;
}

void ABDungeonGenerator::CheckOverlappedRooms()
{
    LatestRoom->BoxComponent->GetOverlappingComponents(OverlappedList);

    if (!OverlappedList.IsEmpty())
    {
        OverlappedList.Empty();
        LatestRoom->Destroy();
        SpawnNextRoom();
    }
    else
    {
        OverlappedList.Empty();
    }

    LatestRoom->ExitComponent->GetChildrenComponents(false, SpawnDirectionList);
    RoomCount++;

    if (RoomCount < 6)
    {
        SpawnNextRoom();
    }
}
