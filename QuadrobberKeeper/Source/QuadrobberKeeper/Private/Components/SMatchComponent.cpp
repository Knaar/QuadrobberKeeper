#include "Components/SMatchComponent.h"

#include "Characters/MyDog.h"
#include "Kismet/GameplayStatics.h"

USMatchComponent::USMatchComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USMatchComponent::InitializeDogs()
{
	// Ищем всех акторов класса AMyDog
	TArray<AActor*> FoundDogs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyDog::StaticClass(), FoundDogs);

	// Очищаем массив перед добавлением новых собак (если требуется)
	DogScoresArray.Empty();

	// Добавляем каждую собаку в массив DogScoresArray со значением очков 0
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

	// Ищем собаку в массиве DogScoresArray
	for (FDogScore& DogScore : DogScoresArray)
	{
		if (DogScore.DogActor == Dog)
		{
			// Если нашли, добавляем к её счёту переданное количество очков
			DogScore.Score += PointsToAdd;

			// Находим текущую собаку-чемпиона
			AActor* CurrentChampion = GetChampionDog();
			
			// Вызов делегата при обновлении счёта
			OnDogScoreChanged.Broadcast(Dog, DogScore.Score,CurrentChampion);

			return; // Завершаем выполнение после обновления счёта
		}
	}

	// Если собака не найдена в массиве, добавляем её
	FDogScore NewDogScore;
	NewDogScore.DogActor = Dog;
	NewDogScore.Score = PointsToAdd;
	DogScoresArray.Add(NewDogScore);

	// Находим текущую собаку-чемпиона
	AActor* CurrentChampion = GetChampionDog();
	
	// Вызов делегата для новой собаки
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

			// Отображаем в логе имя собаки и её счёт
			UE_LOG(LogTemp, Log, TEXT("Dog: %s, Score: %d"), *DogName, Score);
		}
	}
}

void USMatchComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

