// Paridyz


#include "DataAsset_RarityPool.h"

#include "Algo/SelectRandomWeighted.h"

FInstancedStruct UDataAsset_RarityPool::GetRandomItem()
{
	TArray<float> Weights;
	TArray<int32> Indices;
	Indices.Reserve(Items.Num());
	for (int32 i = 0; i < Items.Num(); i++) { Weights.Add(Items[i].Rarity); Indices.Add(i); }
	
	if (Weights.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("UDataAsset_RarityPool::GetRandomItem: Can not get random item because poll is empty!!"));
		return FInstancedStruct();
	}
	
	int32* SelectedIndexPtr = Algo::SelectRandomWeightedBy(Indices, [this](int32 Index)
	{
		return Index;
	});

	return Items[*SelectedIndexPtr].Item;
}