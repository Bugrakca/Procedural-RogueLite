// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralRoguelike/Public/BDungeonGenerator.h"

#include "BDungeonRoom.h"
#include "BRandomSeedComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ABDungeonGenerator::ABDungeonGenerator()
{
    RandomSeedComp = CreateDefaultSubobject<UBRandomSeedComponent>(TEXT("RandomSeedComp"));

    LatestRoom = nullptr;
    RoomCount = 0;
}

// Called when the game starts or when spawned
void ABDungeonGenerator::BeginPlay()
{
    Super::BeginPlay();

    SpawnStartRoom();
}

void ABDungeonGenerator::SpawnStartRoom()
{
    ABDungeonRoom* SpawnRoom = RoomSpawn(SpawnRoomClass, GetTransform());
    SpawnRoom->ExitComponent->GetChildrenComponents(false, SpawnDirectionList);

    for (auto Direction : SpawnDirectionList)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnRoom Directions: %s"), *Direction->GetName());
    }

    RandomSeedComp->InitStream("RoomSelection", 123456, true); //! Refactor: If user enters an input about seed number allow that, if not use random seed.

    //? For Debug usage
    // if(RandomSeedComp->HasStream("WorldGen"))
    // {
    //     int64 Seed = RandomSeedComp->GetStreamSeed("WorldGen");
    // }
    // RandomSeedComp->InitStream("Test2Random");

    LatestRoom = SpawnRoom;

    SpawnNextRoom();
}

void ABDungeonGenerator::SpawnNextRoom()
{
    if (RoomCount > MaxRoomNumber)
    {
        FinalizeDungeon();
        return;
    }

    while (RoomCount <= MaxRoomNumber && SpawnDirectionList.Num() > 0)
    {
        //* Select a random exit from the current room.
        USceneComponent* SelectedExit = SpawnDirectionList[RandomSeedComp->GetRandomIntInRange("RoomSelection", 0, SpawnDirectionList.Num() - 1)];
        UE_LOG(LogTemp, Error, TEXT("Selected Exit: %s"), *SelectedExit->GetName());

        SpawnDirectionList.Remove(SelectedExit); //Remove the selected exit from the list.

        //* Calculate the new room's position and rotation
        FVector ExitDirection = SelectedExit->GetForwardVector();
        float RoomDistance = 750.0f; //* Adding this because the room pivot is at the center.

        FVector SpawnLocation = SelectedExit->GetComponentLocation() + (ExitDirection * RoomDistance);
        FRotator Rotation = SelectedExit->GetComponentTransform().Rotator();
        FVector Scale = FVector(1, 1, 1);
        FTransform SpawnTransform = FTransform(Rotation, SpawnLocation, Scale);

        //* Get random room from the list of rooms
        // const TSubclassOf<ABDungeonRoom> RandomRoom = RoomList[FMath::RandRange(0, RoomList.Num() - 1)];
        const TSubclassOf<ABDungeonRoom> RandomRoom = RoomList[RandomSeedComp->GetRandomIntInRange("RoomSelection", 0, RoomList.Num() - 1)];
        ABDungeonRoom* NewRoom = RoomSpawn(RandomRoom, SpawnTransform);

        // Track this as our latest room
        ABDungeonRoom* PreviousRoom = LatestRoom;
        LatestRoom = NewRoom;

        CheckOverlappedRooms();

        if (!IsValid(LatestRoom))
        {
            LatestRoom = PreviousRoom;

            continue;
        }

        DoorDirectionList.Add(SelectedExit);

        UE_LOG(LogTemp, Warning, TEXT("Spawned room %d at %s"),
               RoomCount, *LatestRoom->GetActorLocation().ToString());

        //Get latest spawned room directions
        TArray<USceneComponent*> NewDirections;
        LatestRoom->ExitComponent->GetChildrenComponents(false, NewDirections);
        SpawnDirectionList.Append(NewDirections);
    }

    if (SpawnDirectionList.Num() == 0 && RoomCount <= MaxRoomNumber)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dungeon generation stopped: No more available exits. Created %d of %d rooms."),
               RoomCount, MaxRoomNumber);
    }

    bIsDungeonComplete = true;
    FinalizeDungeon();
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
        UE_LOG(LogTemp, Warning, TEXT("Room overlaps with existing structures - destroying"));
        OverlappedList.Empty();
        LatestRoom->Destroy();
    }
    else
    {
        RoomCount++;
        OverlappedList.Empty();
    }
}

void ABDungeonGenerator::FinalizeDungeon()
{
    UE_LOG(LogTemp, Warning, TEXT("Finalizing dungeon with %d rooms"), RoomCount);

    //* Spawn walls at remaining open exits
    for (const USceneComponent* RemainingExit : SpawnDirectionList)
    {
        FVector SpawnLocation = RemainingExit->GetComponentLocation();
        FRotator SpawnRotation = RemainingExit->GetComponentRotation() + FRotator(0, 90, 0);
        
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* SpawnedWall = GetWorld()->SpawnActor<AActor>(SpawnWallClass, SpawnLocation, SpawnRotation, Params);

        UE_LOG(LogTemp, Log, TEXT("Wall spawned at exit: %s"), *RemainingExit->GetName());
    }

    UE_LOG(LogTemp, Log, TEXT("Door List Size: %d"), DoorDirectionList.Num());

    //* Spawn doors at connections between rooms
    for (USceneComponent* Door : DoorDirectionList)
    {
        FVector SpawnLocation = Door->GetComponentLocation();
        FRotator SpawnRotation = Door->GetComponentRotation() + FRotator(0, 90, 0);
        
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* SpawnedDoor = GetWorld()->SpawnActor<AActor>(SpawnDoorClass, SpawnLocation, SpawnRotation, Params);
        
        UE_LOG(LogTemp, Log, TEXT("Door spawned at location: %s"), *SpawnLocation.ToString());
    }
}
