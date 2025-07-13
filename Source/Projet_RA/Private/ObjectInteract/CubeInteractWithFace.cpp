// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectInteract/CubeInteractWithFace.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ACubeInteractWithFace::ACubeInteractWithFace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMesh;

	CubeMesh->OnInputTouchBegin.AddDynamic(this, &ACubeInteractWithFace::OnInputTouchBeginCPP);

}

// Called when the game starts or when spawned
void ACubeInteractWithFace::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = GetWorld()->GetFirstPlayerController();
	
}



// Called every frame
void ACubeInteractWithFace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubeInteractWithFace::OnInputTouchBeginCPP(  ETouchIndex::Type ButtonPressed, UPrimitiveComponent* TouchedComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Touch Begin Detected!"));
	if (TouchedComponent == CubeMesh)
	{
		// Handle interaction with the cube face here
		UE_LOG(LogTemp, Warning, TEXT("Cube face touched!"));

		
		if(PlayerControllerRef->IsValidLowLevel())
		{
			//Get the touch location
			FVector2D TouchLocation;
			bool bIsPressed;
			PlayerControllerRef->GetInputTouchState(ButtonPressed, TouchLocation.X, TouchLocation.Y,bIsPressed );
			FVector WorldLocation, WorldDirection;
			PlayerControllerRef->DeprojectScreenPositionToWorld(TouchLocation.X, TouchLocation.Y, WorldLocation, WorldDirection);
			FHitResult HitResult;
			
			
			UKismetSystemLibrary::LineTraceSingleForObjects(
				GetWorld(),
				WorldLocation,
				GetActorLocation(),
				TArray<TEnumAsByte<EObjectTypeQuery>>{ UCollisionProfile::Get()->ConvertToObjectType(ECC_WorldDynamic),
					UCollisionProfile::Get()->ConvertToObjectType(ECC_WorldStatic) }, // Adjust the object type query as needed
				true,
				TArray<AActor*>(),
				EDrawDebugTrace::None,
				HitResult,
				true
			);

			if(HitResult.bBlockingHit)
			{
				// print the hit location
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Hit Location: ") + HitResult.ImpactPoint.ToString());

				FVector UnRotateVector = GetUnrotatedLocation(HitResult.ImpactPoint);

				float X = UKismetMathLibrary::Dot_VectorVector( UnRotateVector, FVector(1,0,0));
				float Y = UKismetMathLibrary::Dot_VectorVector( UnRotateVector, FVector(0,1,0));
				float Z = UKismetMathLibrary::Dot_VectorVector( UnRotateVector, FVector(0,0,1));


				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("X: ") + FString::SanitizeFloat(X) +
					TEXT(" Y: ") + FString::SanitizeFloat(Y) + TEXT(" Z: ") + FString::SanitizeFloat(Z));

				if(X == 1.f)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the right face!"));
				}
				else if(X == -1.f)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the left face!"));
				}
				else if(Y == 1.f)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the top face!"));
				}
				else if(Y == -1.f)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the bottom face!"));
				}
				else if(Z == 1.f)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the front face!"));
				}
				else if(Z == -1.f)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the back face!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No hit detected."));
			}
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Touched component is not the cube mesh."));
	}
}

FVector ACubeInteractWithFace::GetUnrotatedLocation_Implementation(FVector Location)
{ 
	// This function should return the unrotated location based on the cube's rotation.
	// For simplicity, we will just return the input location as is.
	return Location;
}



