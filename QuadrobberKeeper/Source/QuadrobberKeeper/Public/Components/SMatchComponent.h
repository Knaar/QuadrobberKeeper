#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SMatchComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDogScoreChanged, AActor*, Dog, int32, NewScore, AActor*,ChampinDog);

USTRUCT(BlueprintType)
struct FDogScore
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	AActor* DogActor;

	UPROPERTY(BlueprintReadWrite)
	int32 Score;
};



UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUADROBBERKEEPER_API USMatchComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	USMatchComponent();

	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnDogScoreChanged OnDogScoreChanged;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Score")
	TArray<FDogScore> DogScoresArray;

	UFUNCTION(BlueprintCallable, Category = "Initialization")
	void InitializeDogs();
	AActor* GetChampionDog() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScoreToDog(AActor* Dog, int32 PointsToAdd);

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DisplayScores() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
