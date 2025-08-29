// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionInterface.h"
#include "Collectibles/Artefact.h"
#include "Simon.generated.h"

UCLASS()
class PROJET_RA_API ASimon : public AActor , public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "SimonMesh")
	TObjectPtr<UStaticMeshComponent> SimonMesh;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "SimonMesh - Button" )
	TObjectPtr<UStaticMeshComponent> SimonButtonMesh1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "SimonMesh - Button" )
	TObjectPtr<UStaticMeshComponent> SimonButtonMesh2;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "SimonMesh - Button" )
	TObjectPtr<UStaticMeshComponent> SimonButtonMesh3;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "SimonMesh - Button" )
	TObjectPtr<UStaticMeshComponent> SimonButtonMesh4;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "SimonMesh - SceneTP" )
	TObjectPtr<USceneComponent> ScenePlayerTP;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Simon | Sound" )
	TObjectPtr<USoundBase> CorrectButtonSound;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Simon | Sound" )
	TObjectPtr<USoundBase> WrongButtonSound;
	
protected:

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "SimonVariables" )
	TArray<int> SimonSequence;
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "SimonVariables" )
	int CurrentStep;
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "SimonVariables" )
	int SequenceStep = 0;
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "SimonVariables" )
	int MaxSteps = 4; // Number of steps in the Simon sequence

	FTimerHandle SequenceTimerHandle;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "SimonArtefact" )
	TObjectPtr<AArtefact> SimonArtefact;
	
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "SimonVariables" )
	TObjectPtr<APlayerController> PlayerControllerRef;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION( BlueprintCallable, Category = "Simon" )
	void GenerateSimonSequence();

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "Simon" )
	void ColorButton(int StepIndex = 0 , FLinearColor ButtonColor = FLinearColor::White);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Simon" )
	void ColorAllButtons(FLinearColor ButtonColor = FLinearColor::White);

	UFUNCTION( BlueprintCallable, Category = "Simon" )
	void CheckPlayerInput(int PlayerInput);

	UFUNCTION( BlueprintCallable, Category = "Simon" )
	void PlaySimonSequence();

	UFUNCTION( BlueprintCallable, Category = "Simon" )
	void HideActor();

	UFUNCTION( BlueprintCallable, Category = "Simon" )
	void ShowActor();

	UFUNCTION()
	void OnTouchBeginCPP(const ETouchIndex::Type FingerIndex, UPrimitiveComponent* PrimitiveComponent);

public:
	// Sets default values for this actor's properties
	ASimon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
