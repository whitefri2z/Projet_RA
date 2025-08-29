// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectInteract/CubeInteractWithFace.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACubeInteractWithFace::ACubeInteractWithFace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMesh;
	CubeMesh->SetSimulatePhysics(true);
	CubeMesh->SetEnableGravity(false);
	CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CubeMesh->SetCollisionObjectType(ECC_PhysicsBody);

	CubeMesh->OnInputTouchBegin.AddDynamic(this, &ACubeInteractWithFace::OnInputTouchBeginCPP);

	ScenePlayerTP = CreateDefaultSubobject<USceneComponent>(TEXT("ScenePlayerTP"));
	ScenePlayerTP->SetupAttachment(RootComponent);

	HexaedreFaceNormals.Add( FVector(0, 0, 1) ); // Front
	HexaedreFaceNormals.Add( FVector(0, 0, -1) ); // Back
	HexaedreFaceNormals.Add( FVector(1, 0, 0) ); // Right
	HexaedreFaceNormals.Add( FVector(-1, 0, 0) ); // Left
	HexaedreFaceNormals.Add( FVector(0, 1, 0) ); // Top
	HexaedreFaceNormals.Add( FVector(0, -1, 0) ); // Bottom
	
	


}

// Called when the game starts or when spawned
void ACubeInteractWithFace::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = GetWorld()->GetFirstPlayerController();

	// GetRandom face to touch
	SelectNewFace();
	
}



// Called every frame
void ACubeInteractWithFace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if(bIsTouching2)
	{
		float x,y;
		bool bIsPressed;
		PlayerControllerRef->GetInputTouchState(ETouchIndex::Type::Touch2, x, y, bIsPressed);
		if(!bIsPressed)
		{
			bIsTouching2 = false;
			CubeMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
			return;
		}
		CurrentTouchPos = FVector2D(x, y);

		RotateMesh(CurrentTouchPos - PreviousTouchPos);
		PreviousTouchPos = CurrentTouchPos;
	}
	



	

}

void ACubeInteractWithFace::OnInputTouchBeginCPP(  ETouchIndex::Type ButtonPressed, UPrimitiveComponent* TouchedComponent)
{
	if (TouchedComponent == CubeMesh && ButtonPressed == ETouchIndex::Touch1)
	{
		if((GetActorLocation() - PlayerControllerRef->GetPawn()->GetActorLocation()).Size() > 1000.f)
		{
			IInteractionInterface::Execute_FocusOnActor( PlayerControllerRef->GetPawn(), ScenePlayerTP->GetComponentLocation());
			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the cube mesh!"));
			// Call the function to verify interaction with the cube face
			// Pass the ButtonPressed parameter to the function

			TimerVerifyInteractionDelegate = FTimerDelegate::CreateUObject(this, &ACubeInteractWithFace::VerifyInteractionWithFace, ButtonPressed);
		
			GetWorldTimerManager().SetTimer( TimerVerifyInteraction, TimerVerifyInteractionDelegate, 0.3f, false);
		}

	}
	else if (TouchedComponent == CubeMesh && ButtonPressed == ETouchIndex::Touch2)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Touched the cube mesh with Touch2!"));
		// Call the function to verify interaction with the cube face
		// Pass the ButtonPressed parameter to the function
		float x,y;
		bIsTouching2 = true;
		PlayerControllerRef->GetInputTouchState(ETouchIndex::Type::Touch2, x, y, bIsTouching2);
		PreviousTouchPos = FVector2D(x, y);

	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Touched component is not the cube mesh."));
		bIsTouching2 = false;
	}
}

void ACubeInteractWithFace::SetFaceColor_Implementation(FVector NormalFace, FLinearColor Color)
{
}


TArray<FVector> ACubeInteractWithFace::GetNormalisedFaceNormals(TArray<FVector> FaceNormals)
{
	TArray<FVector> Normals;
	for( const FVector& Normal : FaceNormals)
	{
		Normals.Add(Normal.GetSafeNormal());
	}
	return Normals;
}

void ACubeInteractWithFace::VerifyInteractionWithFace(ETouchIndex::Type ButtonPressed)
{
		// Handle interaction with the cube face here
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Verifying interaction with cube face..."));

		if (bIsTouching2)
			return;
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
					UCollisionProfile::Get()->ConvertToObjectType(ECC_WorldStatic), UCollisionProfile::Get()->ConvertToObjectType(ECC_PhysicsBody) }, // Adjust the object type query as needed
				true,
				TArray<AActor*>(),
				EDrawDebugTrace::ForDuration,
				HitResult,
				true
			);

			if(HitResult.bBlockingHit)
			{
				//Get impact Normal in world
				FVector WorldNormal = HitResult.ImpactNormal;
				//Convert to local space
				FVector LocalNormal = GetActorTransform().InverseTransformVectorNoScale(WorldNormal);
				LocalNormal.Normalize();

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("World Normal: ") + WorldNormal.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Local Normal: ") + LocalNormal.ToString());

				//Find the closest face normal
				int ClosestFaceIndex = -1;
				float ClosestDotProduct = -1.0f;

				static TArray< FVector> NormalisedFaceNormals = GetNormalisedFaceNormals(HexaedreFaceNormals);

				for (int i = 0; i < NormalisedFaceNormals.Num(); ++i)
				{
					float DotProduct = FVector::DotProduct(LocalNormal, NormalisedFaceNormals[i]);
					if (DotProduct > ClosestDotProduct)
					{
						ClosestDotProduct = DotProduct;
						ClosestFaceIndex = i;
					}
				}

				if(NormalisedFaceNormals[ClosestFaceIndex] == FaceTouchNormal)
				{
					SetFaceColor( FaceTouchNormal, FLinearColor::Green); // Set the color of the face to green for testing
					// Play correct face sound
					UGameplayStatics::PlaySound2D(GetWorld(), CorrectButtonSound);
					// Reset the color after 0.5 second
					OldTouchFaceNormal = FaceTouchNormal;
					if( InteractwithActorRef && InteractwithActorRef->Implements<UInteractionInterface>())
					{
						VerifPuzzleCompletion();
					}
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Incorrect face touched!"));
					// Play inccorrect face sound
					UGameplayStatics::PlaySound2D(GetWorld(), WrongButtonSound);
					SetFaceColor( NormalisedFaceNormals[ClosestFaceIndex], FLinearColor::Red); // Set the color of the face to green for testing
					OldTouchFaceNormal = FaceTouchNormal;
					// Reset the color after 0.5 second

					GetWorldTimerManager().SetTimer( TimerEffectToFace,  this , &ACubeInteractWithFace::EffectToFace, 0.5f, false);
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No blocking hit detected."));
			}
		}
	else
	{
		GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, TEXT("PlayerControllerRef is not valid."));
	}
}

void ACubeInteractWithFace::RotateMesh(FVector2D Delta)
{
	FVector Torque = FVector(Delta.Y, -Delta.X, 0) * 1.0f; // Adjust the multiplier as needed
	CubeMesh->AddTorqueInRadians(Torque, NAME_None, true);
	// Get the current velocity of the mesh

	
	
}

void ACubeInteractWithFace::SelectNewFace()
{
	// GetRandom face to touch
	FaceTouchNormal = HexaedreFaceNormals[FMath::RandRange(0, HexaedreFaceNormals.Num() - 1)];

	SetFaceColor( FaceTouchNormal, FLinearColor(1.f, 0.5f, 0.f, 1.f)); // Set the color of the face to red for testing
}

void ACubeInteractWithFace::VerifPuzzleCompletion()
{
	RighFaceIndex++;

	if(RighFaceIndex >= 3)
	{
		IInteractionInterface::Execute_SuccessPuzzel(InteractwithActorRef);
		SetActorHiddenInGame(true);
		CubeMesh->SetVisibility(false);
		CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		GetWorldTimerManager().SetTimer( TimerEffectToFace,  this , &ACubeInteractWithFace::SelectNewFace, 0.5f, false);
	}
}

void ACubeInteractWithFace::EffectToFace()
{
	SetFaceColor( OldTouchFaceNormal, FLinearColor(1.f, 0.5f, 0.f, 1.f));
}



