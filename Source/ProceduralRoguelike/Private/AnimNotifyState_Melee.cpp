// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Melee.h"

#include "BCharacter.h"
#include "BWeaponBase.h"
#include "KismetTraceUtils.h"
#include "Animation/AnimSequenceBase.h"
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
    // AActor* MyOwner = MeshComp->GetOwner();
    // if (ABCharacter* MyCharacter = Cast<ABCharacter>(MyOwner))
    // {
    //     CurrentWeapon = MyCharacter->GetCurrentWeapon();
    // }
    UE_LOG(LogTemp, Error, TEXT("=== NOTIFY STATE DEBUG ==="));
    UE_LOG(LogTemp, Error, TEXT("NotifyState Instance: %p"), this);
    UE_LOG(LogTemp, Error, TEXT("Character: %s"), *MeshComp->GetOwner()->GetName());
    UE_LOG(LogTemp, Error, TEXT("Animation: %s"), *Animation->GetName());

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    FMeleeTraceData& Data = CharacterData.FindOrAdd(Owner);

    UE_LOG(LogTemp, Warning, TEXT("=== NOTIFY STATE BEGIN ==="));
    UE_LOG(LogTemp, Warning, TEXT("Character: %s"), *Owner->GetName());

    if (Owner->Implements<UUCombatInterface>())
    {
        Data.CurrentWeapon = IUCombatInterface::Execute_GetCurrentWeapon(Owner);
        Data.WeaponOwner = Owner;

        UE_LOG(LogTemp, Warning, TEXT("Weapon: %s"), Data.CurrentWeapon ? *Data.CurrentWeapon->GetName() : TEXT("NULL"));
        
        if (Data.CurrentWeapon)
        {
            Data.CurrentWeapon->ResetHitActors();
            if (UStaticMeshComponent* WeaponMesh = Data.CurrentWeapon->GetStaticMesh())
            {
                Data.PreviousTopTrace = WeaponMesh->GetSocketLocation(TopSocketName);
                Data.PreviousBottomTrace = WeaponMesh->GetSocketLocation(BottomSocketName);
            }
        }
    }
    UE_LOG(LogTemp, Error, TEXT("============================="));
}

void UAnimNotifyState_Melee::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    UE_LOG(LogTemp, Warning, TEXT("=== NOTIFY STATE END ==="));
    UE_LOG(LogTemp, Warning, TEXT("Character: %s"), *Owner->GetName());

    CharacterData.Remove(Owner);
    
    UE_LOG(LogTemp, Warning, TEXT("============================="));
}

void UAnimNotifyState_Melee::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    FMeleeTraceData* Data = CharacterData.Find(Owner);
    
    if (!Data || !Data->CurrentWeapon || !IsValid(Data->CurrentWeapon))
    {
        UE_LOG(LogTemp, Warning, TEXT("No valid weapon data for %s"), *Owner->GetName());
        return;
    }
    
    UStaticMeshComponent* WeaponMesh = Data->CurrentWeapon->GetStaticMesh();
    if (!IsValid(WeaponMesh))
        return;

    UE_LOG(LogTemp, VeryVerbose, TEXT("NotifyTick - Character: %s, Weapon: %s"), 
               *Owner->GetName(), *Data->CurrentWeapon->GetName());
    
    FVector CurrentTopTrace = WeaponMesh->GetSocketLocation("TopTrace");
    FVector CurrentBottomTrace = WeaponMesh->GetSocketLocation("BottomTrace");
    
    // Calculate center point between sockets
    FVector CurrentCenter = (CurrentTopTrace + CurrentBottomTrace) / 2.0f;
    FVector PreviousCenter = (Data->PreviousTopTrace + Data->PreviousBottomTrace) / 2.0f;
    
    float WeaponLength = FVector::Dist(CurrentTopTrace, CurrentBottomTrace);
    
    FVector HalfSize = FVector(WeaponLength / 2.0f, 10.0f, 10.0f);
    
    FVector WeaponDirection = (CurrentTopTrace - CurrentBottomTrace).GetSafeNormal();
    FRotator WeaponRotation = FRotationMatrix::MakeFromX(WeaponDirection).Rotator();
    
    FCollisionShape CollisionShape = FCollisionShape::MakeBox(HalfSize);
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);
    QueryParams.bTraceComplex = false;
    
    TArray<FHitResult> Hits;
    bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(Hits, PreviousCenter, CurrentCenter, WeaponRotation.Quaternion(), TraceChannel, CollisionShape, QueryParams);
#if !UE_BUILD_SHIPPING
    if (DevelopmentOnly::GDrawDebugMelee)
    {
        DrawDebugBoxTraceMulti(MeshComp->GetWorld(), PreviousCenter, CurrentCenter, HalfSize, WeaponRotation, EDrawDebugTrace::ForDuration, bHit, Hits, FLinearColor::Red, FLinearColor::Green, 1.0f);
    }
#endif
    
    Data->PreviousTopTrace = CurrentTopTrace;
    Data->PreviousBottomTrace = CurrentBottomTrace;

    for (const FHitResult& Hit : Hits)
    {
        if (bHit)
        {
            Data->CurrentWeapon->TryApplyDamage(Hit);
        }
    }
}
