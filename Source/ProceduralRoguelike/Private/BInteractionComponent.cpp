﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "BInteractionComponent.h"

#include "BCharacter.h"
#include "BGameplayInterface.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UBInteractionComponent::UBInteractionComponent()
{
    
}


void UBInteractionComponent::PrimaryInteract()
{
    const UCameraComponent* CameraComp = nullptr;

    FCollisionObjectQueryParams Params;
    Params.AddObjectTypesToQuery(ECC_WorldDynamic);

    AActor* MyOwner = GetOwner();

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(MyOwner);

    if (ABCharacter* MyCharacter = Cast<ABCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
    {
        CameraComp = MyCharacter->GetCameraComponent();
    }

    FCollisionShape Shape;
    Shape.SetSphere(20.0f);

    FVector CameraLocation = CameraComp->GetComponentLocation();
    FRotator CameraRotation = CameraComp->GetComponentRotation();

    FVector End = CameraLocation + (CameraRotation.Vector() * 500.0f);

    TArray<FHitResult> Hits;

    bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, CameraLocation, End, FQuat::Identity, Params, Shape, QueryParams);

    FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

    for (FHitResult Hit : Hits)
    {
        if (AActor* HitActor = Hit.GetActor())
        {
            if (HitActor->Implements<UBGameplayInterface>())
            {
                APawn* MyPawn = Cast<APawn>(MyOwner);
                IBGameplayInterface::Execute_Interact(HitActor, MyPawn);
            }
        }
        DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.0f, 20, LineColor, false, 2.0f);
    }

    DrawDebugLine(GetWorld(), CameraLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}
