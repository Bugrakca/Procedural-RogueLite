// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BWeaponBase.generated.h"

class UCurveFloat;
class UTimelineComponent;
class UAudioComponent;
class UCameraShakeBase;
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
    TObjectPtr<USceneComponent> SceneRootComp;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> StaticMeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> BoxComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UAudioComponent> AudioComp;

    UPROPERTY(BlueprintReadOnly)
    bool bDamageApplied = false;

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

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DamageAmount;

public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void WeaponAttackStart();
    
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void WeaponAttackEnd();

protected:
    UFUNCTION()
    virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
