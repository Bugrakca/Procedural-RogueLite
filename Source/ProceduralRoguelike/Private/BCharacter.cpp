// Fill out your copyright notice in the Description page of Project Settings.


#include "BCharacter.h"

#include "BAttributeComponent.h"
#include "BInputConfigData.h"
#include "BInteractionComponent.h"
#include "BWeaponBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ABCharacter::ABCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->bUsePawnControlRotation = true;
    SpringArmComp->SetupAttachment(RootComponent);

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(SpringArmComp);

    InteractionComp = CreateDefaultSubobject<UBInteractionComponent>(TEXT("InteractionComp"));

    AttributesComp = CreateDefaultSubobject<UBAttributeComponent>(TEXT("AttributesComp"));

    GetCharacterMovement()->bOrientRotationToMovement = true;

    bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ABCharacter::BeginPlay()
{
    Super::BeginPlay();
    AttachWeapon();
}

// Called every frame

void ABCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input

void ABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    APlayerController* PC = GetController<APlayerController>();

    UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
    check(SubSystem);

    SubSystem->ClearAllMappings();
    SubSystem->AddMappingContext(PlayerMappingContext, 0);

    UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    EnhancedInputComp->BindAction(InputConfigData->IAction_Move, ETriggerEvent::Triggered, this, &ABCharacter::Move);
    EnhancedInputComp->BindAction(InputConfigData->IAction_Look, ETriggerEvent::Triggered, this, &ABCharacter::Look);
    EnhancedInputComp->BindAction(InputConfigData->IAction_Interact, ETriggerEvent::Completed, this, &ABCharacter::PrimaryInteract);
}

void ABCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);

    if (MovementVector.X != 0.0f)
    {
        FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
        AddMovementInput(MovementDirection, MovementVector.X);
    }

    if (MovementVector.Y != 0.0f)
    {
        FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
        AddMovementInput(MovementDirection, MovementVector.Y);
    }
}

void ABCharacter::Look(const FInputActionValue& Value)
{
    FVector2D RotationValue = Value.Get<FVector2D>();

    AddControllerYawInput(RotationValue.X);
    AddControllerPitchInput(RotationValue.Y);
}

void ABCharacter::PrimaryInteract()
{
    InteractionComp->PrimaryInteract();
}

void ABCharacter::AttachWeapon()
{
     FTransform WeaponTransform = GetMesh()->GetSocketTransform("WeaponSocketR");
     FTransform SpawnTM = WeaponTransform;

     FActorSpawnParameters SpawnParams;
     SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

     if (!CurrentWeapon && WeaponClass)
     {
         CurrentWeapon = GetWorld()->SpawnActor<ABWeaponBase>(WeaponClass, SpawnTM, SpawnParams);

         if (CurrentWeapon)
         {
             CurrentWeapon->SetOwner(this);

             CurrentWeapon->AttachToComponent(GetMesh(), AttachmentTransformRules, "WeaponSocketR");
         }
     }
}

UCameraComponent* ABCharacter::GetCameraComponent()
{
    return CameraComp;
}

ABWeaponBase* ABCharacter::GetCurrentWeapon_Implementation() const
{
    if (CurrentWeapon)
        return CurrentWeapon;

    return nullptr;
}
