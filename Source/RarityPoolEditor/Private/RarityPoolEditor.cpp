#include "RarityPoolEditor.h"

#include "Asset/AssetTypeActions_RarityPool.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FRarityPoolEditorModule"

void FRarityPoolEditorModule::StartupModule()
{		
	UE_LOG(LogTemp, Display, TEXT("FRarityPoolEditorModule startup!"));
    TSharedPtr<IAssetTypeActions> RarityPoolActions = MakeShareable(new FAssetTypeActions_RarityPool);
	if (RarityPoolActions && FModuleManager::LoadModulePtr<FAssetToolsModule>("AssetTools"))
	{
		FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get().RegisterAssetTypeActions(RarityPoolActions.ToSharedRef());	
	}
}

void FRarityPoolEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FRarityPoolEditorModule, RarityPoolEditor)