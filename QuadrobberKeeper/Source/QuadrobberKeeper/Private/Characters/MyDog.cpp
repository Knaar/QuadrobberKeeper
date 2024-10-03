// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MyDog.h"

#include "Characters/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AMyDog::AMyDog()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AMyDog::RotateMeshTowardsTarget(UStaticMeshComponent* MeshComp)
{
	if(!MeshComp) return;
	FVector MeshLock = MeshComp ->GetComponentLocation();
	AActor* NearestActor = nullptr;
	float SearchDist = FLT_MAX;

	if(bHunting&&ToyClass)
	{
		TArray <AActor*> FoundToys;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),ToyClass,FoundToys);

		for(auto* Toy :FoundToys )
		{
			if (Toy)
			{
				float DistanceSquared = FVector::DistSquared(MeshLock,Toy->GetActorLocation());

				if (DistanceSquared <SearchDist)
				{
					SearchDist = DistanceSquared;
					NearestActor = Toy;
				}
			}
		}
	}
	
	else
	{
		TArray<AActor*> FoundedCharacters;
    
		
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyCharacter::StaticClass(), FoundedCharacters);

		for (auto Character : FoundedCharacters)
		{
			if (Character)
			{
				float DistSquared = FVector::DistSquared(MeshLock, Character->GetActorLocation());
				if (DistSquared < SearchDist)
				{
					SearchDist = DistSquared;
					NearestActor = Character;
				}
			}
		}
	}
	
	if (NearestActor)
	{
		FVector TargetLocation = NearestActor->GetActorLocation();
		FVector DirectionToTarget = TargetLocation - MeshLock;
		DirectionToTarget.Z = 0;
		
		FRotator TargetRotation = DirectionToTarget.Rotation();
		
		MeshComp->SetWorldRotation(TargetRotation);
	}
	
	
}

bool AMyDog::FindDogsAndChaseToy()
{
	if (!ToyClass || !DogClass) return false;

	float MaxTimeToReachToy = 0.0f;
	AActor* LastDogToReachToy = nullptr;
	
	TArray<AActor*> FoundToys;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ToyClass, FoundToys);

	if (FoundToys.Num() == 0) return false; 
	AActor* ToyActor = FoundToys[0];

	TArray<AActor*> FoundDogs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), DogClass, FoundDogs);

	for (AActor* DogActor : FoundDogs)
	{
		if (ACharacter* DogCharacter = Cast<ACharacter>(DogActor))
		{
			UCharacterMovementComponent* MovementComponent = DogCharacter->GetCharacterMovement();
			if (MovementComponent)
			{
				float DogSpeed = MovementComponent->MaxWalkSpeed;
				float DistanceToToy = FVector::Dist(DogActor->GetActorLocation(), ToyActor->GetActorLocation());
				float TimeToReachToy = DistanceToToy / DogSpeed;
				if (TimeToReachToy > MaxTimeToReachToy)
				{
					MaxTimeToReachToy = TimeToReachToy;
					LastDogToReachToy = DogActor;
				}

			}
		}
	}
	return (LastDogToReachToy != this);
}
