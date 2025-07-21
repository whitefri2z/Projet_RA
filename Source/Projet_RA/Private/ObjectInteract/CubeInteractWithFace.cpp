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

	// GetRandom face to touch
	//FaceCubeTouched = static_cast<EFaceCube>(FMath::RandRange(0, static_cast<int32>(EFaceCube::Bottom)));

	FaceCubeTouched = EFaceCube::Right; // For testing purposes, we set it to Right face
	
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
				WorldLocation + WorldDirection * 1000.f, // Adjust the length of the trace as needed
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
				//Get Hit Cube face
				FVector HitNormal = HitResult.ImpactNormal;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Hit Normal: ") + HitNormal.ToString());
				if(HitNormal.X == 1)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the right face!"));
					if(FaceCubeTouched == EFaceCube::Right)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Correct face touched!"));
						if( InteractwithActorRef && InteractwithActorRef->Implements<UInteractionInterface>())
						{
							IInteractionInterface::Execute_SuccessPuzzel(InteractwithActorRef);
							SetActorHiddenInGame(true);
							CubeMesh->SetVisibility(false);
							CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						}
					}
				}
				else if (HitNormal.X == -1)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the left face!"));
					if(FaceCubeTouched == EFaceCube::Left)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Correct face touched!"));
						if( InteractwithActorRef && InteractwithActorRef->Implements<UInteractionInterface>())
						{
							IInteractionInterface::Execute_SuccessPuzzel(InteractwithActorRef);
							SetActorHiddenInGame(true);
							CubeMesh->SetVisibility(false);
							CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						}
					}
				}
				else if (HitNormal.Y == 1)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the top face!"));
					if(FaceCubeTouched == EFaceCube::Top)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Correct face touched!"));
						if( InteractwithActorRef && InteractwithActorRef->Implements<UInteractionInterface>())
						{
							IInteractionInterface::Execute_SuccessPuzzel(InteractwithActorRef);
							SetActorHiddenInGame(true);
							CubeMesh->SetVisibility(false);
							CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						}
					}
				}
				else if (HitNormal.Y == -1)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the bottom face!"));
					if(FaceCubeTouched == EFaceCube::Bottom)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Correct face touched!"));
						if( InteractwithActorRef && InteractwithActorRef->Implements<UInteractionInterface>())
						{
							IInteractionInterface::Execute_SuccessPuzzel(InteractwithActorRef);
							SetActorHiddenInGame(true);
							CubeMesh->SetVisibility(false);
							CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						}
					}
				}
				else if (HitNormal.Z == 1)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the front face!"));
					if(FaceCubeTouched == EFaceCube::Front)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Correct face touched!"));
						if( InteractwithActorRef && InteractwithActorRef->Implements<UInteractionInterface>())
						{
							IInteractionInterface::Execute_SuccessPuzzel(InteractwithActorRef);
							SetActorHiddenInGame(true);
							CubeMesh->SetVisibility(false);
							CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						}
					}
				}
				else if (HitNormal.Z == -1)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the back face!"));
					if(FaceCubeTouched == EFaceCube::Back)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Correct face touched!"));
						if( InteractwithActorRef && InteractwithActorRef->Implements<UInteractionInterface>())
						{
							IInteractionInterface::Execute_SuccessPuzzel(InteractwithActorRef);
							SetActorHiddenInGame(true);
							CubeMesh->SetVisibility(false);
							CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						}
					}
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



