// Paridyz

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_RarityPool.h"
#include "AssetFactory_RarityPool.generated.h"

/**
 * 
 */
UCLASS()
class RARITYPOOLEDITOR_API UAssetFactory_RarityPool : public UFactory
{
	GENERATED_BODY()
	
	UAssetFactory_RarityPool();
	
public:
	
	virtual bool ConfigureProperties() override;

	virtual UClass* ResolveSupportedClass() override
	{
		return UDataAsset_RarityPool::StaticClass();
	}

	virtual bool ShouldShowInNewMenu() const override
	{
		return true;
	}
	
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
