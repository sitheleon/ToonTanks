// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h" // HealthComponent 헤더 파일을 포함합니다.
#include "Components/ActorComponent.h" // ActorComponent 포함

ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
if(TankPlayerController)
{
    FHitResult HitResult;
    TankPlayerController->GetHitResultUnderCursor(
        ECollisionChannel::ECC_Visibility,
        false,
        HitResult
    );
      DrawDebugSphere(
            GetWorld(),
            HitResult.ImpactPoint,
            25.f,
            12,
            FColor::Red,
            false,
            -1.f);



     RotateTurret(HitResult.ImpactPoint);


    }
}


void ATank::HandleDestruction()
{
    Super::HandleDestruction();

    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    bAlive = false;

}

void ATank::BeginPlay()
{
    Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());

        // 블루프린트에서 할당된 컴포넌트에 대한 포인터를 가져옵니다.
    HealthComponent = FindComponentByClass<UHealthComponent>();

    if (HealthComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthComponent Found!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HealthComponent not found!"));
    }

}





void ATank::Move(float Value)
{
   FVector DeltaLocation = FVector::ZeroVector;

     DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation, true);
     GetController();
}

void ATank::Turn(float Value)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;
    // Yaw = Value * DeltaTime * TurnRate
    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
}

float ATank::GetHealthPercent() const
{
    if (HealthComponent) // HealthComponent가 nullptr이 아닌지 확인
    {
        float Health = HealthComponent->GetCurrentHealth(); // 세미콜론 추가
        float MaxHealth = HealthComponent->GetMaxHealth();  // 세미콜론 추가
        return Health / MaxHealth;
    }
    
    // HealthComponent가 nullptr일 경우를 대비한 기본 반환값
    return 0.0f;
}
