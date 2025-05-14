// Fill out your copyright notice in the Description page of Project Settings.


#include "BRandomSeedComponent.h"


// Sets default values for this component's properties
UBRandomSeedComponent::UBRandomSeedComponent()
{
    
}

void UBRandomSeedComponent::InitStream(FName StreamName, int64 UserSeed, bool bUseUserSeed)
{
    const int64 FinalSeed = bUseUserSeed ? UserSeed : GenerateRandomSeed();

    FRandomPcg32 NewRNG;
    NewRNG.Initialize(static_cast<uint64>(FinalSeed));
    
    RNGMap.Add(StreamName, NewRNG);
    SeedMap.Add(StreamName, FinalSeed);
    
#if WITH_EDITOR
    if (GEngine)
    {
        const FString Msg = FString::Printf(TEXT("PCG Stream '%s' Seed: %lld"), *StreamName.ToString(), FinalSeed);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Msg);
    }
#endif
    
}

int32 UBRandomSeedComponent::GetRandomInt(FName StreamName, int32 Max)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
    if(!IsInGameThread())
    {
        UE_LOG(LogTemp, Warning, TEXT("RNG accessed from non-game thread!"));
        // Stack trace for debugging
        FDebug::DumpStackTraceToLog(ELogVerbosity::Warning); 
    }
#endif
    
    if (Max <= 0)
    {
        Max = INT32_MAX;
    }
    
    if (FRandomPcg32* RNG = RNGMap.Find(StreamName))
    {
        return static_cast<int32>(RNG->RandRange(static_cast<uint32>(Max)));
    }

    UE_LOG(LogTemp, Warning, TEXT("Stream '%s' not found!"), *StreamName.ToString());
    return 0;
}

int32 UBRandomSeedComponent::GetRandomIntInRange(FName StreamName, int32 Min, int32 Max)
{
    if (FRandomPcg32* RNG = RNGMap.Find(StreamName))
    {
        return Min + static_cast<int32>(RNG->RandRange(static_cast<uint32>(Max - Min)));
    }

    UE_LOG(LogTemp, Warning, TEXT("Stream '%s' not found!"), *StreamName.ToString());
    return Min;
}

int64 UBRandomSeedComponent::GetStreamSeed(FName StreamName) const
{
    if (const int64* Seed = SeedMap.Find(StreamName))
    {
        return *Seed;
    }
    return 0;
}

bool UBRandomSeedComponent::HasStream(FName StreamName) const
{
    return RNGMap.Contains(StreamName);
}

int64 UBRandomSeedComponent::GenerateRandomSeed() const
{
    const int64 TimeStamp = FDateTime::UtcNow().ToUnixTimestamp() * 1000; // Millisecond precision
    const int64 Cycles = static_cast<int64>(FPlatformTime::Cycles64());
    const int64 Rand = FMath::Rand(); // Additional entropy
    
    return TimeStamp ^ (Cycles << 32) ^ Rand;
}