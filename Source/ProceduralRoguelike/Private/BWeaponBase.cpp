// Fill out your copyright notice in the Description page of Project Settings.


#include "BWeaponBase.h"

#include "BAttributeComponent.h"
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

void ABWeaponBase::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bDamageApplied)
    {
        return;
    }

    if (IsValid(OtherActor))
    {
        UBAttributeComponent* AttributeComp = Cast<UBAttributeComponent>(OtherActor->GetComponentByClass(UBAttributeComponent::StaticClass()));
        if (IsValid(AttributeComp))
        {
            FVector End = OtherActor->GetActorLocation();
            
            FHitResult Hit;
            GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), End, ECC_WorldDynamic);

            AttributeComp->ApplyHealthChange(-DamageAmount);

            UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Hit.ImpactPoint);

            UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactVfx, Hit.Location, Hit.ImpactNormal.Rotation(), FVector(1), true,true);

            UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
        }
    }

    bDamageApplied = true;
}

void ABWeaponBase::BeginPlay()
{
    Super::BeginPlay();

    BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ABWeaponBase::OnActorOverlap);
}

void ABWeaponBase::WeaponAttackStart_Implementation()
{
    bDamageApplied = false;
    BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABWeaponBase::WeaponAttackEnd_Implementation()
{
    BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



