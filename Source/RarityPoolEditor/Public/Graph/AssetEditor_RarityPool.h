// Paridyz

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_RarityPool.h"
#include "UObject/Object.h"

class FAssetEditor_RarityPool : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
public:

	void InitAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, class UDataAsset_RarityPool* Asset);
	
	void CreateEdGraph();
	
	UPROPERTY()
	UDataAsset_RarityPool* EditingAsset;
	
	// --- IToolkit Interface ---
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetReferencerName() const override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	void ExtendToolbar();
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);
	
	TSharedRef<SDockTab> SpawnTab_GraphPanelv2(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_DetailsPanel(const FSpawnTabArgs& Args);
	
	void AddComment();
	void OnTextChangeCommited(const FText& Text, ETextCommit::Type Type, UEdGraphNode* Node);
	void OnEnableSelectedNodes();
	void OnDisableSelectedNodes();
	void DeleteSelectedNodes();
	void OnSelectedNodesChanged(const FGraphPanelSelectionSet& SelectionSet);
	bool CanDeleteNodes();
	
	void CompileGraph();
	
	TSharedPtr<SGraphEditor> GraphEditor;
	
protected:
	TSharedPtr<class IDetailsView> DetailsViewWidget;
	
private:
	TSharedPtr<SDockTab> GraphPanelTab;
	TSharedPtr<FUIAction> Compile;
	//URarityPollGraphWidget* RarityPollGraphWidget; // From my UE5.8.3 code
};