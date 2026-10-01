// Paridyz

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "StructUtils/InstancedStruct.h"
#include "DataAsset_RarityPool.generated.h"

USTRUCT(BlueprintType)
struct FItemDefiner
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInstancedStruct Item;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Rarity = 0.0f;
};

// Soft object path items
USTRUCT(BlueprintType)
struct FSoftRarityItem
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FSoftObjectPath Item;
};

// Gameplay tag items
USTRUCT(BlueprintType)
struct FTagRarityItem
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag Item;
};

UCLASS()
class RARITYPOOL_API UDataAsset_RarityPool : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FInstancedStruct GetRandomItem();
	
	UPROPERTY()
	TArray<FItemDefiner> Items;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UObject>> AllowedClasses;
	
	UPROPERTY()
	UEdGraph* EditorGraph;
#endif
};
