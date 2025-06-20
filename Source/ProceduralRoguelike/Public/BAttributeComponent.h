// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UBAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROCEDURALROGUELIKE_API UBAttributeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UBAttributeComponent();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    float Health;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    float HealthMax;

    //Stamina, Strength etc.

public:
    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnHealthChanged;
    
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    bool ApplyHealthChange(float Delta);
};
