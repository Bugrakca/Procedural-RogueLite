// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "BCharacter.generated.h"

class UBInteractionComponent;
class UInputMappingContext;
class UBInputConfigData;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROCEDURALROGUELIKE_API ABCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ABCharacter();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<USpringArmComponent> SpringArmComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UCameraComponent> CameraComp;

    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    TObjectPtr<UBInputConfigData> InputConfigData;

    UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
    TObjectPtr<UInputMappingContext> PlayerMappingContext;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UBInteractionComponent> InteractionComp;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void Move(const FInputActionValue& Value);

    void Look(const FInputActionValue& Value);

    void PrimaryInteract();

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UCameraComponent* GetCameraComponent();
};
