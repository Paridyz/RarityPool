// Paridyz


#include "Asset/AssetTypeActions_RarityPool.h"
#include "Graph/AssetEditor_RarityPool.h"

#include "DataAsset_RarityPool.h"

FText FAssetTypeActions_RarityPool::GetName() const
{
	return FText::FromString("Rarity Pool");
}

UClass* FAssetTypeActions_RarityPool::GetSupportedClass() const
{
	return UDataAsset_RarityPool::StaticClass();
}

FColor FAssetTypeActions_RarityPool::GetTypeColor() const
{
	return FColor(60, 138, 240);
}

uint32 FAssetTypeActions_RarityPool::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

void FAssetTypeActions_RarityPool::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* Object : InObjects)
	{
		if (UDataAsset_RarityPool* RarityPool = Cast<UDataAsset_RarityPool>(Object))
		{
			TSharedRef<FAssetEditor_RarityPool> NewEditor(new FAssetEditor_RarityPool());
			NewEditor->InitAssetEditor(Mode, EditWithinLevelEditor, RarityPool);
		}
	}
}
