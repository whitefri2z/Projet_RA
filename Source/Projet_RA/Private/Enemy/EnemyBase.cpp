// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"

#include "Enemy/EnemyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Network/GameStateAR.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemySkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemySkeletal"));
	RootComponent = EnemySkeletal;

	EnemySkeletal->OnInputTouchBegin.AddDynamic(this, &AEnemyBase::OnTouchEnemy);

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBase::ActionCarcolh()
{
}

void AEnemyBase::ActionDrak()
{
	if(DrakDistance > 100)
	{
		if(bIsVisibleDrak)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Drak is now hidden!"));
		
			ShowEnemy();
			bIsVisibleDrak = false;
			SetActorLocation( FVector(0.0f, DrakDistance * (FMath::RandBool() ? 1 : -1) , 0.0f)); // Reset position or set to a specific location
		}
		else
		{
			HideEnemy();
			bIsVisibleDrak = true;
			DrakDistance -= 50.0f; // Decrease the distance by 50 units each time the action is triggered
		
		}
		GetWorldTimerManager().SetTimer( EnemySpawnTimerHandle, this, &AEnemyBase::ActionDrak, 2.0f, false);
	}
	else
	{
		ShowEnemy();
		bIsVisibleDrak = false;
		SetActorLocation( FVector(50.0f, 0 , -50.0f)); // Reset position or set to a specific location
	}
}

void AEnemyBase::ActionLoupDrape()
{
	ThrowProjectile();
}

void AEnemyBase::ThrowProjectile()
{
	// Implement projectile throwing logic here
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Throwing projectile!"));

	
	for (AEnemyProjectile * Projectile : EnemyProjectile)
	{
		if (Projectile)
		{
			if(Projectile->GetIsUsed())
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Projectile is already used!"));
				continue; // Skip this projectile if it's already used
			}
			// Set the projectile's properties, such as location, rotation, etc.
			Projectile->SetActorLocation(GetActorLocation() + GetActorForwardVector() * FVector(50, 0 ,50)); // Adjust the spawn location as needed
			Projectile->SetActorRotation(GetActorRotation());
			// Activate the projectile if needed
			Projectile->SetActorEnableCollision(true);
			Projectile->ProjectileMesh->SetVisibility( true);
			Projectile->SetActorHiddenInGame(false);
			// Optionally, you can set the projectile's velocity or other properties here
			Projectile->ProjectileMovementComponent->Activate(); // Activate the projectile movement component
			Projectile->ProjectileMovementComponent->SetUpdatedComponent(Projectile->ProjectileMesh); // Set the component to update
			// Set the initial velocity of the projectile
			FRotator ProjectileRotationWithPlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
			Projectile->SetActorRotation(ProjectileRotationWithPlayer);			
			Projectile->ProjectileMovementComponent->Velocity = GetActorForwardVector() * 500.0f; // Example velocity
			Projectile->ProjectileMovementComponent->UpdateComponentVelocity();
			Projectile->SetIsUsed(true); // Mark the projectile as used
			break;
			
		}
	}
	
	// You can spawn a projectile actor here and set its properties

	GetWorldTimerManager().SetTimer(EnemySpawnTimerHandle, this, &AEnemyBase::ThrowProjectile, 2.0f, false);
}

void AEnemyBase::ActionTarasque()
{
}

void AEnemyBase::UpdateEnemyMesh_Implementation()
{
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

 void AEnemyBase::HideEnemy()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	GetWorldTimerManager().ClearTimer( EnemySpawnTimerHandle); // Clear the timer to stop further actions

	if(!bIsVisibleDrak)
	GameStateARRef->WaitSpawnEnemies();
}

 void AEnemyBase::ShowEnemy()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void AEnemyBase::InitEnemy(EEnemyType NewEnemyType, AGameStateAR* GameStateAR)
{
	EnemyType = NewEnemyType;
	GameStateARRef = GameStateAR;
	UpdateEnemyMesh();

	bIsActive = true;

	// Example: Set the enemy to be visible or perform some action
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Enemy is now visible and enabled!"));

	// Initialize enemy rotation for look player
	//Get Rotation to look at player
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	SetActorRotation( LookAtRotation);
	
	
	switch (EnemyType) {
	case EEnemyType::Carcolh:
		break;
	case EEnemyType::Drak:
		bIsVisibleDrak = true;
		DrakDistance = 500.0f; // Set initial distance for Drak
		GetWorldTimerManager().SetTimer( EnemySpawnTimerHandle, this, &AEnemyBase::ActionDrak, 2.0f, false);
		break;
	case EEnemyType::Loup_Drape:
		GetWorldTimerManager().SetTimer( EnemySpawnTimerHandle, this, &AEnemyBase::ActionLoupDrape, 2.0f, false);
		break;
	case EEnemyType::Tarasque:
		break;
	}
}

void AEnemyBase::ActorHitByProjectile_Implementation()
{
	IInteractionInterface::ActorHitByProjectile_Implementation();

	LoupDrapeHitCount++;
	if(LoupDrapeHitCount >= 3) // Assuming 3 hits to defeat the enemy
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Loup Drape defeated!"));
		bIsActive = false;
		HideEnemy();
		LoupDrapeHitCount = 0; // Reset hit count after defeat
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Loup Drape hit! Remaining hits: ") + FString::FromInt(3 - LoupDrapeHitCount));
	}
}

void AEnemyBase::OnTouchEnemy(ETouchIndex::Type ButtonPressed, UPrimitiveComponent* TouchedComponent)
{
	 GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Yellow, TEXT("Enemy touched!"));
	switch (EnemyType) {
	case EEnemyType::Carcolh:
		break;
	case EEnemyType::Drak:
		if (ButtonPressed == ETouchIndex::Touch1 && bIsTakingDamage) // Assuming Touch1 is the primary touch
		{
			if(DraktakeDamage > 0)
			{
				DraktakeDamage--;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Drak took damage! Remaining: ") + FString::FromInt(DraktakeDamage));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Drak is defeated!"));
				bIsActive = false;
				HideEnemy();
			}
		}
		break;
	case EEnemyType::Loup_Drape:
		break;
	case EEnemyType::Tarasque:
		break;
	}
}
