#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyDog.generated.h"

UCLASS()
class QUADROBBERKEEPER_API AMyDog : public ACharacter
{
	GENERATED_BODY()

public:
	
	AMyDog();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category=BaseSettings)
	TSubclassOf<AActor> ToyClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category=BaseSettings)
	TSubclassOf<AActor> DogClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bHunting = false;

	UFUNCTION(BlueprintCallable, Category = "Utilities")
	void RotateMeshTowardsTarget(UStaticMeshComponent* MeshComp);
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool FindDogsAndChaseToy();
};
