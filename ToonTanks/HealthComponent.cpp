// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksGameMode.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHealthComponent::SetMaxHealth(float amount)
{
	MaxHealth = MaxHealth + amount;

	UE_LOG(LogTemp, Warning, TEXT("Changed Max Health: %f"), MaxHealth);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Health = MaxHealth;
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay in HealthComponent: %s"), *GetOwner()->GetName());

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);

	ToonTanksGameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser )
{
	 UE_LOG(LogTemp, Warning, TEXT("DamageTaken called on %s with damage: %f"), *DamagedActor->GetName(), Damage);

	if(Damage <= 0.f) return;

	Health -= Damage;
	
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
	
	if(Health <= 0.f && ToonTanksGameMode)
	{
		ToonTanksGameMode->ActorDied(DamagedActor);

	}
}

void UHealthComponent::Heal(float Amount)
{
    if (Amount <= 0.f) 
	{
	return;
	}

    Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);

    UE_LOG(LogTemp, Warning, TEXT("Health after healing: %f"), Health);
}
