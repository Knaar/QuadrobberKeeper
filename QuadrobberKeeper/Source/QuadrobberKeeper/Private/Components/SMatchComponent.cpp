#include "Components/SMatchComponent.h"

#include "Characters/MyDog.h"
#include "Kismet/GameplayStatics.h"

USMatchComponent::USMatchComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USMatchComponent::InitializeDogs()
{
	TArray<AActor*> FoundDogs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyDog::StaticClass(), FoundDogs);
	
	DogScoresArray.Empty();
	
	for (AActor* Dog : FoundDogs)
	{
		FDogScore NewDogScore;
		NewDogScore.DogActor = Dog;
		NewDogScore.Score = 0;

		DogScoresArray.Add(NewDogScore);
	}
}

AActor* USMatchComponent::GetChampionDog() const
{
	const FDogScore* ChampionDog = nullptr;

	for (const FDogScore& DogScore : DogScoresArray)
	{
		if (!ChampionDog || DogScore.Score > ChampionDog->Score)
		{
			ChampionDog = &DogScore;
		}
	}

	return ChampionDog ? ChampionDog->DogActor : nullptr;
}

void USMatchComponent::AddScoreToDog(AActor* Dog, int32 PointsToAdd)
{
	if (!Dog) return;
	
	for (FDogScore& DogScore : DogScoresArray)
	{
		if (DogScore.DogActor == Dog)
		{
			DogScore.Score += PointsToAdd;
			
			AActor* CurrentChampion = GetChampionDog();
			
			OnDogScoreChanged.Broadcast(Dog, DogScore.Score,CurrentChampion);

			return;
		}
	}
	
	FDogScore NewDogScore;
	NewDogScore.DogActor = Dog;
	NewDogScore.Score = PointsToAdd;
	DogScoresArray.Add(NewDogScore);
	
	AActor* CurrentChampion = GetChampionDog();
	
	OnDogScoreChanged.Broadcast(Dog, PointsToAdd,CurrentChampion);
}

void USMatchComponent::DisplayScores() const
{
	for (const FDogScore& DogScore : DogScoresArray)
	{
		if (DogScore.DogActor)
		{
			FString DogName = DogScore.DogActor->GetName();
			int32 Score = DogScore.Score;
			UE_LOG(LogTemp, Log, TEXT("Dog: %s, Score: %d"), *DogName, Score);
		}
	}
}

void USMatchComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

AActor* USMatchComponent::GetChampionDogOrNullIfTie() const
{
	const FDogScore* ChampionDog = nullptr;
	bool bIsTie = false;
	
	for (const FDogScore& DogScore : DogScoresArray)
	{
		if (!ChampionDog)
		{
			ChampionDog = &DogScore;
			bIsTie = false;
		}
		else if (DogScore.Score > ChampionDog->Score)
		{
			ChampionDog = &DogScore;
			bIsTie = false;
		}
		else if (DogScore.Score == ChampionDog->Score)
		{
			bIsTie = true;
		}
	}
	
	return bIsTie ? nullptr : ChampionDog->DogActor;
}