// Fill out your copyright notice in the Description page of Project Settings.


#include "BWeaponBase.h"

#include "BAttributeComponent.h"
#include "BCharacter.h"
#include "KismetTraceUtils.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABWeaponBase::ABWeaponBase()
{
    SceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRootComp);

    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    BoxComp->SetupAttachment(RootComponent);

    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    StaticMeshComp->SetupAttachment(RootComponent);

    AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
    AudioComp->SetupAttachment(RootComponent);

    DamageAmount = 20;
}

UStaticMeshComponent* ABWeaponBase::GetStaticMesh()
{
    return StaticMeshComp;
}

void ABWeaponBase::PerformWeaponTrace()
{
    if (bDamageApplied)
    {
        return;
    }

    if (bWeaponTrace)
    {
        UStaticMeshComponent* WeaponMesh = StaticMeshComp;
        if (IsValid(WeaponMesh))
        {
            FVector CurrentTopTrace = WeaponMesh->GetSocketLocation("TopTrace");
            FVector CurrentBottomTrace = WeaponMesh->GetSocketLocation("BottomTrace");

            float Distance = FVector::Dist(CurrentTopTrace, CurrentBottomTrace);

            FRotator SocketRotation = FRotationMatrix::MakeFromX(CurrentBottomTrace - CurrentTopTrace).Rotator();

            FVector HalfSize = FVector(Distance / 2.0f, 10.0f, 10.0f);

            FCollisionShape CollisionShape = FCollisionShape::MakeBox(HalfSize);

            FCollisionQueryParams QueryParams;
            QueryParams.AddIgnoredActor(GetOwner());

            FHitResult Hit;
            bool bHit = GetWorld()->SweepSingleByChannel(Hit, PreviousTopTrace, CurrentTopTrace, SocketRotation.Quaternion(), ECC_Visibility, CollisionShape, QueryParams);
            DrawDebugBoxTraceSingle(GetWorld(), PreviousTopTrace, CurrentTopTrace, HalfSize, SocketRotation, EDrawDebugTrace::ForDuration, bHit, Hit, FLinearColor::Red, FLinearColor::Green, 1.0f);

            PreviousTopTrace = CurrentTopTrace;
            PreviousBottomTrace = CurrentBottomTrace;

            if (bHit)
            {
                if (Hit.GetActor())
                {
                    UBAttributeComponent* AttributeComponent = Hit.GetActor()->GetComponentByClass<UBAttributeComponent>();
                    if (IsValid(AttributeComponent))
                    {
                        AttributeComponent->ApplyHealthChange(-DamageAmount);

                        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.ImpactPoint);

                        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactVfx, Hit.Location, Hit.ImpactNormal.Rotation(), FVector(1), true, true);

                        UGameplayStatics::PlayWorldCameraShake(GetWorld(), ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);

                        bDamageApplied = true;
                    }
                }
            }
        }
    }
}

void ABWeaponBase::ToggleWeaponTrace(bool bWeaponTraceOn)
{
    bWeaponTrace = bWeaponTraceOn;

    if (bWeaponTrace)
    {
        UStaticMeshComponent* WeaponMesh = StaticMeshComp;
        if (IsValid(WeaponMesh))
        {
            PreviousTopTrace = WeaponMesh->GetSocketLocation("TopTrace");
            PreviousBottomTrace = WeaponMesh->GetSocketLocation("BottomTrace");
        }
    }
}

void ABWeaponBase::WeaponAttackStart_Implementation()
{
    BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABWeaponBase::WeaponAttackEnd_Implementation()
{
    bDamageApplied = false;
    BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
