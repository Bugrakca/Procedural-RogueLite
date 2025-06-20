// Fill out your copyright notice in the Description page of Project Settings.


#include "BWeaponBase.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABWeaponBase::ABWeaponBase()
{
    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    RootComponent = BoxComp;

    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    StaticMeshComp->SetupAttachment(RootComponent);

    AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
    AudioComp->SetupAttachment(RootComponent);
}

void ABWeaponBase::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    DamageVfx();
}

void ABWeaponBase::BeginPlay()
{
    Super::BeginPlay();

    BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ABWeaponBase::OnActorOverlap);
}

UBoxComponent* ABWeaponBase::GetBoxComponent() const
{
    return BoxComp;
}

void ABWeaponBase::DamageVfx_Implementation()
{
    if (ensure(IsValid(this)))
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactVfx, GetActorLocation(), GetActorRotation(), FVector(1), true,true);

        UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

        UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
    }
}



