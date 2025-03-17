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
}

// Called every frame

void ABDungeonGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABDungeonGenerator::SpawnStartRoom()
{
    ABDungeonRoom* SpawnRoom = RoomSpawn(SpawnRoomClass, GetTransform());
    SpawnRoom->ExitComponent->GetChildrenComponents(false, SpawnDirectionList);

    for (auto Direction : SpawnDirectionList)
    {
        UE_LOG(LogTemp, Error, TEXT("Direction: %s"), *Direction->GetName());
    }

    LatestRoom = SpawnRoom;

    SpawnNextRoom();
}

void ABDungeonGenerator::SpawnNextRoom()
{
    if (RoomCount > 6)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dungeon complete with %d rooms"), RoomCount);
        return;
    }

    if (SpawnDirectionList.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("No exits found for room %d"), RoomCount);
        return;
    }

    USceneComponent* SelectedExit = SpawnDirectionList[FMath::RandRange(0, SpawnDirectionList.Num() - 1)];
    UE_LOG(LogTemp, Error, TEXT("Selected Exit: %s"), *SelectedExit->GetName());

    // Calculate new room position and rotation
    FVector ExitDirection = SelectedExit->GetForwardVector();
    float RoomDistance = 1500.0f;

    //Room transform
    FVector SpawnLocation = LatestRoom->GetActorLocation() + (ExitDirection * RoomDistance);

    FRotator Rotation = SelectedExit->GetComponentTransform().Rotator();

    FVector Scale = FVector(1, 1, 1);

    FTransform SpawnTransform = FTransform(Rotation, SpawnLocation, Scale);

    // Choose room type - Boss room for last room, regular rooms otherwise
    // TSubclassOf<ABDungeonRoom> RoomClassToSpawn;
    // if (RoomCount == 5) // Last room (0-indexed count)
    // {
    //     RoomClassToSpawn = BossRoomClass;
    // }
    // else
    // {
    //     RoomClassToSpawn = RegularRoomClass;
    // }
    SpawnDirectionList.Remove(SelectedExit);

    // Spawn the new room
    ABDungeonRoom* NewRoom = RoomSpawn(RegularRoomClass, SpawnTransform);

    // Track this as our latest room
    ABDungeonRoom* PreviousRoom = LatestRoom;
    LatestRoom = NewRoom;

    
    // Check for overlaps with existing rooms
    CheckOverlappedRooms();

    //Get spawn directions
    TArray<USceneComponent*> NewDirections;
    LatestRoom->ExitComponent->GetChildrenComponents(false, NewDirections);

    SpawnDirectionList.Append(NewDirections);
    
    for (auto Direction : SpawnDirectionList)
    {
        UE_LOG(LogTemp, Log, TEXT("Directions: %s"), *Direction->GetName());
    }

    // If the room was destroyed due to overlap, try again
    if (!IsValid(LatestRoom))
    {
        // Restore previous room as latest and try again
        LatestRoom = PreviousRoom;
        SpawnNextRoom();
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Spawned room %d at %s"),
           RoomCount, *LatestRoom->GetActorLocation().ToString());

    // Continue to next room
    SpawnNextRoom();
}

ABDungeonRoom* ABDungeonGenerator::RoomSpawn(UClass* SpawnClass, const FTransform& Transform)
{
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ABDungeonRoom* SpawnedRoom = GetWorld()->SpawnActor<ABDungeonRoom>(SpawnClass, Transform, Params);

    return SpawnedRoom;
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
        RoomCount++;
        OverlappedList.Empty();
    }
}
