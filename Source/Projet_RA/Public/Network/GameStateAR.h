// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateAR.generated.h"

class AEnemyBase;
/**
 * 
 */
UCLASS()
class PROJET_RA_API AGameStateAR : public AGameStateBase
{
	GENERATED_BODY()

protected:

	//Reference to the enemy base actor
	UPROPERTY(EditAnywhere ,BlueprintReadWrite, Category = "GameState")
	TObjectPtr<UClass> EnemyBase;

	//Timer handle for spawning enemies
	FTimerHandle EnemySpawnTimerHandle;

	FTimerDelegate EnemySpawnDelegate;

	

	// Function to spawn enemies at regular intervals
	UFUNCTION(BlueprintCallable, Category = "GameState")
	void ShowEnemies(TArray<AActor*> AllEnemy);

	UFUNCTION(BlueprintCallable, Category = "GameState")
	void HideEnemies(TArray<AActor*> AllEnemy);

	UFUNCTION( BlueprintCallable, Category = "GameState")
	FVector RandomLocationEnemies();
	
	virtual void BeginPlay() override;


	
};
