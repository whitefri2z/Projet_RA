// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CustomPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_RA_API ACustomPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	int ArtefactsCollected = 0;


public:

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void AddArtefact()
	{
		ArtefactsCollected++;
		GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Orange, FString::Printf(TEXT("Artefact collected! Total: %d"), ArtefactsCollected));
		OnArtefactCollected();
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Artefact")
	void OnArtefactCollected();

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	int GetArtefactsCollected() const{return ArtefactsCollected;}

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void SetArtefactsCollected(int NewCount) { ArtefactsCollected = NewCount; }
	
	
	
};
