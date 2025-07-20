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

	

protected:

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "CubeVariables")
	TObjectPtr<APlayerController> PlayerControllerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeVariables")
	EFaceCube FaceCubeTouched;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CubeVariables")
	TObjectPtr<AActor> InteractwithActorRef;


	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "CubeInteraction")
	void OnInputTouchBeginCPP(ETouchIndex::Type ButtonPressed, UPrimitiveComponent* TouchedComponent);

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "CubeInteraction")
	FVector GetUnrotatedLocation(FVector Location);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	ACubeInteractWithFace();

};
