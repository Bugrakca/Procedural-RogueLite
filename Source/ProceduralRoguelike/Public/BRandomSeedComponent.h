// Fill out your copyright notice in the Description page of Project Settings.☺

#pragma once

#include "CoreMinimal.h"
#include "FRandomPcg32.hpp"
#include "Components/ActorComponent.h"
#include "BRandomSeedComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROCEDURALROGUELIKE_API UBRandomSeedComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UBRandomSeedComponent();

    /** Initialize a random stream with optional seed */
    UFUNCTION(BlueprintCallable, Category = "RNG")
    void InitStream(FName StreamName, int64 UserSeed = 0, bool bUseUserSeed = false);

    /** Get random int in range [0, Max) */
    UFUNCTION(BlueprintCallable, Category = "RNG")
    int32 GetRandomInt(FName StreamName, int32 Max);

    /** Get random int in range [Min, Max) */
    UFUNCTION(BlueprintCallable, Category = "RNG")
    int32 GetRandomIntInRange(FName StreamName, int32 Min, int32 Max);

    /** Get the seed value for a stream */
    UFUNCTION(BlueprintCallable, Category = "RNG")
    int64 GetStreamSeed(FName StreamName) const;

    /** Check if stream exists */
    UFUNCTION(BlueprintCallable, Category = "RNG")
    bool HasStream(FName StreamName) const;

private:
    TMap<FName, FRandomPcg32> RNGMap;
    TMap<FName, int64> SeedMap;

    int64 GenerateRandomSeed() const;

};
