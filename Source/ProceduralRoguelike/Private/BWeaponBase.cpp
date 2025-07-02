// Fill out your copyright notice in the Description page of Project Settings.


#include "BWeaponBase.h"

#include "BAttributeComponent.h"
#include "BCharacter.h"
#include "KismetTraceUtils.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraShakeBase.h"
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

void ABWeaponBase::ResetHitActors()
{
    HitActors.Empty();
    UE_LOG(LogTemp, Log, TEXT("Reset hit actors for weapon: %s"), *GetName());
}

bool ABWeaponBase::TryApplyDamage(const FHitResult& Hit)
{
    if (!Hit.GetActor())
        return false;

    AActor* HitActor = Hit.GetActor();

    if (HitActors.Contains(HitActor))
    {
        UE_LOG(LogTemp, Log, TEXT("Actor %s already hit, skipping"), *HitActor->GetName());
        return false;
    }

    UBAttributeComponent* AttributeComponent = HitActor->GetComponentByClass<UBAttributeComponent>();
    if (!IsValid(AttributeComponent))
    {
        return false;
    }

    HitActors.Add(HitActor);
    AttributeComponent->ApplyHealthChange(-DamageAmount);
    PlayImpactEffects(Hit);

    return true;
}

void ABWeaponBase::PlayImpactEffects(const FHitResult& Hit)
{
    if (ImpactSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.ImpactPoint);
    }

    if (ImpactVfx)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactVfx, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), FVector(1), true, true);
    }

    if (ImpactShake)
    {
        UGameplayStatics::PlayWorldCameraShake(GetWorld(), ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
    }
}
