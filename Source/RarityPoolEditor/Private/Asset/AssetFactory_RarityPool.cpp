// Paridyz


#include "Asset/AssetFactory_RarityPool.h"

UAssetFactory_RarityPool::UAssetFactory_RarityPool()
{
	bCreateNew = true;
	bEditAfterNew = true;	
	SupportedClass = UDataAsset_RarityPool::StaticClass();
}

bool UAssetFactory_RarityPool::ConfigureProperties()
{
	return Super::ConfigureProperties();
}

UObject* UAssetFactory_RarityPool::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
	EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{	
	return NewObject<UDataAsset_RarityPool>(InParent, InClass, InName, Flags);
}
