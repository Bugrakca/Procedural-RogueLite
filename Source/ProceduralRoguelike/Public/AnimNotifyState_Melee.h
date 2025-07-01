// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_Melee.generated.h"

class ABWeaponBase;
enum ECollisionChannel : int;
/**
 * 
 */
UCLASS()
class PROCEDURALROGUELIKE_API UAnimNotifyState_Melee : public UAnimNotifyState
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere)
    TEnumAsByte<ECollisionChannel> TraceChannel;

    UPROPERTY(EditAnywhere)
    FName TopSocketName;

    UPROPERTY(EditAnywhere)
    FName BottomSocketName;

    UPROPERTY()
    TObjectPtr<ABWeaponBase> CurrentWeapon;

    FVector PreviousTopTrace;
    
    FVector PreviousBottomTrace;

public:
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

    virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

    UAnimNotifyState_Melee();
};
