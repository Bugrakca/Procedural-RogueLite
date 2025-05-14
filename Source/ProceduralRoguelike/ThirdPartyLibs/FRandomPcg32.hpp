#pragma once

#include "CoreMinimal.h"

/**
 * PCG32 Random Number Generator
 * 
 * A fast, high-quality random number generator based on the PCG algorithm.
 * PCG (Permuted Congruential Generator) combines a linear congruential generator
 * with output permutation for better statistical properties.
 */

class PROCEDURALROGUELIKE_API FRandomPcg32
{
public:
    /** Constants from original PCG design */
    static constexpr uint64 PCG_DEFAULT_MULTIPLIER = 6364136223846793005ULL;
    static constexpr uint64 PCG_DEFAULT_INCREMENT = 0xda3e39cb94b95bdbULL;

    /** Default constructor initializes with a time-based seed */
    FRandomPcg32()
    {
        const uint64 TimeSeed = FPlatformTime::Cycles64();
        Initialize(TimeSeed);
    }

    /** Constructor with explicit seed and optional stream */
    explicit FRandomPcg32(uint64 InSeed, uint64 InStream = PCG_DEFAULT_INCREMENT)
    {
        Initialize(InSeed, InStream);
    }

    /** Seed the generator */
    void Initialize(uint64 InSeed, uint64 InStream = PCG_DEFAULT_INCREMENT)
    {
        State = 0;
        Increment = (InStream << 1u) | 1u;
        
        // Warm-up
        RandUInt32();
        
        State += InSeed;
        
        // Finalize initialization
        RandUInt32();
    }

    /** Get a 32-bit random unsigned integer */
    uint32 RandUInt32() noexcept
    {
        const uint64 OldState = State;
        State = OldState * PCG_DEFAULT_MULTIPLIER + Increment;

        // XOR shift and rotate operations for output
        const uint32 XorShifted = static_cast<uint32>(((OldState >> 18u) ^ OldState) >> 27u);
        const uint32 Rotation = static_cast<uint32>(OldState >> 59u);

        // Calculate rotation amount (handles negative rotation)
        const uint32 RotationAmount = (static_cast<uint32>(-static_cast<int32>(Rotation)) & 31);

        // Apply rotation
        return (XorShifted >> Rotation) | (XorShifted << RotationAmount);

    }

    /** Get a random unsigned integer in range [0, InMax) */
    uint32 RandRange(uint32 Max) noexcept
    {
        return Max > 0 ? RandUInt32() % Max : 0;
    }

    /** Get a random unsigned integer in range [InMin, InMax) */
    uint32 RandRange(uint32 Min, uint32 Max) noexcept
    {
        if (Min >= Max)
        {
            return Min;
        }
        
        return Min + RandRange(Max - Min);
    }

    /** Get a random float in range [0.0f, 1.0f) */
    float RandFloat()
    {
        const uint32 RandomInt = RandUInt32();

        return static_cast<float>(RandomInt) * (1.0f / 4294967296.0f);
    }

    /** Get a random float in range [InMin, InMax) */
    float RandRange(float Min, float Max) noexcept
    {
        if (Min >= Max)
        {
            return Min;
        }

        return Min + (Max - Min) * RandFloat();
    }

    /** Get a random bool with specified probability */
    bool RandBool(float Probability = 0.5f) noexcept
    {
        // Clamp probability to valid range
        Probability = FMath::Clamp(Probability, 0.0f, 1.0f);

        return RandFloat() < Probability;
    }

private:
    uint64 State = 0;
    uint64 Increment = PCG_DEFAULT_INCREMENT;
};
