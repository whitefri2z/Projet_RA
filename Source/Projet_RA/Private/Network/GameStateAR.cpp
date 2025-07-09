// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/GameStateAR.h"

#include "Enemy/EnemyBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"


void AGameStateAR::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> AllEnemy;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyBase, AllEnemy);
	
	
	
	// Initialize the enemy base actor
	if (!AllEnemy.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EnemyBase is valid!"));
		// Set up the timer delegate to call ShowEnemies function
		EnemySpawnDelegate.BindUFunction(this, FName("ShowEnemies"), AllEnemy);
		// Set the timer to call ShowEnemies every 5 seconds
		GetWorldTimerManager().SetTimer(EnemySpawnTimerHandle, EnemySpawnDelegate, 5.0f, true);
	}
}

void AGameStateAR::ShowEnemies( TArray<AActor*> AllEnemy)
{
	if (!AllEnemy.IsEmpty())
	{
		for (AActor* Enemy : AllEnemy)
		{
			if (Enemy)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Enemy: %s"), *Enemy->GetName()));
				// You can add more logic here to handle the enemy actors
				AEnemyBase *EnemyShow = Cast<AEnemyBase>(Enemy);
				if (EnemyShow)
				{
					// Example: Set the enemy to be visible or perform some action
					EnemyShow->SetActorHiddenInGame(false);
					EnemyShow->SetActorEnableCollision(true);
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Enemy is now visible and enabled!"));
					// Optionally, you can call a function on the enemy to perform some action
					EnemyShow->SetActorLocation( RandomLocationEnemies());
					
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to cast to AEnemyBase!"));
				}
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No enemies found!"));
	}
		
}

void AGameStateAR::HideEnemies(TArray<AActor*> AllEnemy)
{
}

FVector AGameStateAR::RandomLocationEnemies()
{
	FVector RandomLocation;

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	
	// Generate a random location within a certain range
	RandomLocation.X = FMath::RandRange(-1000.0f, 1000.0f);
	RandomLocation.Y = FMath::RandRange(-1000.0f, 1000.0f);
	RandomLocation.Z = 0.0f; // Assuming a flat ground for simplicity
	
	return RandomLocation + PlayerLocation;
}
