// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Melee.h"

#include "BCharacter.h"
#include "BWeaponBase.h"
#include "KismetTraceUtils.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"

#if !UE_BUILD_SHIPPING
namespace DevelopmentOnly
{
    static bool GDrawDebugMelee = false;
    static FAutoConsoleVariableRef CVarDrawDebug_MeleeNotifies(
        TEXT("game.drawdebugmelee"),
        GDrawDebugMelee,
        TEXT("Enable debug rendering on the melee system.\n"),
        ECVF_Cheat
        );
}
#endif

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
    
    // Calculate center point between sockets
    FVector CurrentCenter = (CurrentTopTrace + CurrentBottomTrace) / 2.0f;
    FVector PreviousCenter = (PreviousTopTrace + PreviousBottomTrace) / 2.0f;
    
    float WeaponLength = FVector::Dist(CurrentTopTrace, CurrentBottomTrace);
    
    FVector HalfSize = FVector(WeaponLength / 2.0f, 10.0f, 10.0f);
    
    FVector WeaponDirection = (CurrentTopTrace - CurrentBottomTrace).GetSafeNormal();
    FRotator WeaponRotation = FRotationMatrix::MakeFromX(WeaponDirection).Rotator();
    
    FCollisionShape CollisionShape = FCollisionShape::MakeBox(HalfSize);
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(MeshComp->GetOwner());
    QueryParams.bTraceComplex = false;
    
    TArray<FHitResult> Hits;
    bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(Hits, PreviousCenter, CurrentCenter, WeaponRotation.Quaternion(), TraceChannel, CollisionShape, QueryParams);
#if !UE_BUILD_SHIPPING
    if (DevelopmentOnly::GDrawDebugMelee)
    {
        DrawDebugBoxTraceMulti(MeshComp->GetWorld(), PreviousCenter, CurrentCenter, HalfSize, WeaponRotation, EDrawDebugTrace::ForDuration, bHit, Hits, FLinearColor::Red, FLinearColor::Green, 1.0f);
    }
#endif
    PreviousTopTrace = CurrentTopTrace;
    PreviousBottomTrace = CurrentBottomTrace;

    for (const FHitResult& Hit : Hits)
    {
        if (bHit)
        {
            CurrentWeapon->TryApplyDamage(Hit);
        }
    }
}
