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
		/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EnemyBase is valid!"));
		// Set up the timer delegate to call ShowEnemies function
		EnemySpawnDelegate.BindUFunction(this, FName("ShowEnemies"), AllEnemy);
		// Set the timer to call ShowEnemies every 5 seconds
		GetWorldTimerManager().SetTimer(EnemySpawnTimerHandle, EnemySpawnDelegate, 5.0f, false );*/
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
					// Optionally, you can call a function on the enemy to perform some action
					EnemyShow->SetActorLocation( RandomLocationEnemies());
					
					EEnemyType RandomEnemyType = static_cast<EEnemyType>(FMath::RandRange(0, static_cast<int32>(EEnemyType::Tarasque)));
					
					// Initialize the enemy with a specific type and reference to this game state
					EnemyShow->InitEnemy( RandomEnemyType, this); // Example of initializing the enemy with a type and reference to this game state
					
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
	FVector PlayerForwardVector = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorForwardVector();
	
	// Generate a random location within a certain range
	RandomLocation.X = FMath::RandRange(200.f, 1000.0f);
	RandomLocation.Y = FMath::RandRange(200.f, 1000.0f) * (FMath::RandBool() ? 1.0f : -1.0f); // Randomly choose positive or negative Y
	RandomLocation.Z = 0.0f; // Assuming a flat ground for simplicity

	RandomLocation *= PlayerForwardVector; // Scale the random location by the player's forward vector to ensure it's in front of the player

	
	return RandomLocation + PlayerLocation;
}
