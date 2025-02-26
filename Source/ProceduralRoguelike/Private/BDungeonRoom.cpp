// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralRoguelike/Public/BDungeonRoom.h"

#include "Components/BoxComponent.h"


// Sets default values
ABDungeonRoom::ABDungeonRoom()
{
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    RootComponent = BoxComponent;

    ExitComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Exits"));
    ExitComponent->SetupAttachment(BoxComponent);
}


