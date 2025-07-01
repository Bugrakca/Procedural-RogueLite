// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Melee.h"

#include "BCharacter.h"
#include "BWeaponBase.h"
#include "KismetTraceUtils.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"

UAnimNotifyState_Melee::UAnimNotifyState_Melee()
{
    TraceChannel = ECC_Pawn;
}

void UAnimNotifyState_Melee::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    // Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
    AActor* MyOwner = MeshComp->GetOwner();
    if (ABCharacter* MyCharacter = Cast<ABCharacter>(MyOwner))
    {
        CurrentWeapon = MyCharacter->GetCurrentWeapon();
    }

    if (CurrentWeapon)
    {
        CurrentWeapon->ResetHitActors();
        if (UStaticMeshComponent* WeaponMesh = CurrentWeapon->GetStaticMesh())
        {
            PreviousTopTrace = WeaponMesh->GetSocketLocation(TopSocketName);
            PreviousBottomTrace = WeaponMesh->GetSocketLocation(BottomSocketName);
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Weapon trace started, weapon: %s"), CurrentWeapon ? *CurrentWeapon->GetName() : TEXT("None"));
}

void UAnimNotifyState_Melee::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    // Super::NotifyEnd(MeshComp, Animation, EventReference);
    UE_LOG(LogTemp, Warning, TEXT("Weapon trace ended"));

    // CurrentWeapon = nullptr;
}

void UAnimNotifyState_Melee::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    if (!CurrentWeapon)
        return;

    UStaticMeshComponent* WeaponMesh = CurrentWeapon->GetStaticMesh();
    if (!IsValid(WeaponMesh))
        return;
    
    FVector CurrentTopTrace = WeaponMesh->GetSocketLocation("TopTrace");
    FVector CurrentBottomTrace = WeaponMesh->GetSocketLocation("BottomTrace");
    
    float Distance = FVector::Dist(CurrentTopTrace, CurrentBottomTrace);
    FRotator SocketRotation = FRotationMatrix::MakeFromX(CurrentBottomTrace - CurrentTopTrace).Rotator();
    FVector HalfSize = FVector(Distance / 2.0f, 10.0f, 10.0f);

    FCollisionShape CollisionShape = FCollisionShape::MakeBox(HalfSize);
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(MeshComp->GetOwner());
    
    QueryParams.bTraceComplex = false;

    FHitResult Hit;
    bool bHit = MeshComp->GetWorld()->SweepSingleByChannel(Hit, PreviousTopTrace, CurrentTopTrace, SocketRotation.Quaternion(), TraceChannel, CollisionShape, QueryParams);

    DrawDebugBoxTraceSingle(MeshComp->GetWorld(), PreviousTopTrace, CurrentTopTrace, HalfSize, SocketRotation, EDrawDebugTrace::ForDuration, bHit, Hit, FLinearColor::Red, FLinearColor::Green, 1.0f);
    
    PreviousTopTrace = CurrentTopTrace;
    PreviousBottomTrace = CurrentBottomTrace;
    
    if (bHit)
    {
        CurrentWeapon->TryApplyDamage(Hit);
    }
}
