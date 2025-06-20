// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BWeaponBase.generated.h"

class UNiagaraSystem;
class UBoxComponent;

UCLASS()
class PROCEDURALROGUELIKE_API ABWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABWeaponBase();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> StaticMeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> BoxComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UAudioComponent> AudioComp;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TObjectPtr<UNiagaraSystem> ImpactVfx;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TObjectPtr<USoundBase> ImpactSound;

    UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
    TSubclassOf<UCameraShakeBase> ImpactShake;

    UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
    float ImpactShakeInnerRadius;

    UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
    float ImpactShakeOuterRadius;
    

    UFUNCTION()
    virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void DamageVfx();

public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    UBoxComponent* GetBoxComponent() const;
};
