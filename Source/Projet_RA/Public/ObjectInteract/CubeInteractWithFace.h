// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionInterface.h"
#include "CubeInteractWithFace.generated.h"

UENUM(BlueprintType)
enum class EFaceCube : uint8 
{
	Front UMETA(DisplayName = "Front" ),
	Back UMETA(DisplayName = "Back" ),
	Left UMETA(DisplayName = "Left" ),
	Right UMETA(DisplayName = "Right" ),
	Top UMETA(DisplayName = "Top" ),
	Bottom UMETA(DisplayName = "Bottom" )
};


UCLASS()
class PROJET_RA_API ACubeInteractWithFace : public AActor , public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "CubeMesh")
	TObjectPtr<UStaticMeshComponent> CubeMesh;

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "SceneTP")
	TObjectPtr<USceneComponent> ScenePlayerTP;


	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cube | Sound" )
	TObjectPtr<USoundBase> CorrectButtonSound;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cube | Sound" )
	TObjectPtr<USoundBase> WrongButtonSound;

protected:

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "CubeVariables")
	TObjectPtr<APlayerController> PlayerControllerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeVariables")
	EFaceCube FaceCubeTouched;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CubeVariables")
	TObjectPtr<AActor> InteractwithActorRef;

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "CubeVariables")
	int RighFaceIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeVariables")
	FVector FaceTouchNormal;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "CubeVariables")
	FVector OldTouchFaceNormal;

	TArray<FVector> HexaedreFaceNormals;

	bool bIsTouching2 = false;

	FVector2D PreviousTouchPos;
	FVector2D CurrentTouchPos;

	FTimerHandle TimerVerifyInteraction;
	FTimerDelegate TimerVerifyInteractionDelegate;

	FTimerHandle TimerEffectToFace;


	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "CubeInteraction")
	void OnInputTouchBeginCPP(ETouchIndex::Type ButtonPressed, UPrimitiveComponent* TouchedComponent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ColorFace")
	void SetFaceColor(FVector NormalFace, FLinearColor Color);

	UFUNCTION( BlueprintCallable, Category = "CubeInteraction")
	static TArray<FVector> GetNormalisedFaceNormals(TArray<FVector> FaceNormals);
	

	UFUNCTION( BlueprintCallable, Category = "CubeInteraction")
	void VerifyInteractionWithFace(ETouchIndex::Type ButtonPressed);

	UFUNCTION( BlueprintCallable, Category = "CubeInteraction")
	void RotateMesh(FVector2D Delta);

	UFUNCTION(BlueprintCallable, Category = "CubeInteraction")
	void SelectNewFace();

	UFUNCTION( BlueprintCallable, Category = "CubeInteraction")
	void VerifPuzzleCompletion();

	UFUNCTION( BlueprintCallable, Category = "CubeInteraction")
	void EffectToFace();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	ACubeInteractWithFace();

};
