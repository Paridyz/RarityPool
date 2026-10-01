// Paridyz


#include "Graph/AssetEditor_RarityPool.h"

#include "EdGraphNode_Comment.h"
#include "Graph/EdGraphNode_RarityPollItem.h"
#include "GraphEditorActions.h"
#include "Graph/GraphSchema_RarityPool.h"
#include "Framework/Commands/GenericCommands.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "RarityPoolEditor"

FName FAssetEditor_RarityPool::GetToolkitFName() const
{
	return FName("RarityPoolEditor");
}

FText FAssetEditor_RarityPool::GetBaseToolkitName() const
{
	return INVTEXT("Rarity Pool Editor");
}

FString FAssetEditor_RarityPool::GetWorldCentricTabPrefix() const
{
	return TEXT("RarityPool");
}

FLinearColor FAssetEditor_RarityPool::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

FString FAssetEditor_RarityPool::GetReferencerName() const
{
	return "RarityPoolEditor";
}


void FAssetEditor_RarityPool::InitAssetEditor(const EToolkitMode::Type Mode,
                                              const TSharedPtr<IToolkitHost>& InitToolkitHost, class UDataAsset_RarityPool* Asset)
{
    EditingAsset = Asset;
    
    FGenericCommands::Register();
    FGraphEditorCommands::Register();
	
	ToolkitCommands->MapAction(
		FGenericCommands::Get().Delete,
		FExecuteAction::CreateSP(this, &FAssetEditor_RarityPool::DeleteSelectedNodes),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_RarityPool::CanDeleteNodes)
	);
	
	ToolkitCommands->MapAction(
		FGraphEditorCommands::Get().CreateComment,
		FExecuteAction::CreateSP(this, &FAssetEditor_RarityPool::AddComment)
	);
	
	ToolkitCommands->MapAction(
		FGraphEditorCommands::Get().DisableNodes,
		FExecuteAction::CreateSP(this, &FAssetEditor_RarityPool::OnDisableSelectedNodes)
	);
	
	ToolkitCommands->MapAction(
		FGraphEditorCommands::Get().EnableNodes,
		FExecuteAction::CreateSP(this, &FAssetEditor_RarityPool::OnEnableSelectedNodes)
	);
	
	ToolkitCommands->MapAction(
		FGraphEditorCommands::Get().EnableNodes_Always,
		FExecuteAction::CreateSP(this, &FAssetEditor_RarityPool::OnEnableSelectedNodes)
	);

	ExtendToolbar();
	
    const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_RarityPool_Layout_v1")
			->AddArea
			(
				// Main application area
				FTabManager::NewPrimaryArea()
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewSplitter()
					->SetOrientation(Orient_Horizontal)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.25f)
						->AddTab("RarityPoolDetails", ETabState::OpenedTab)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetHideTabWell(false)
						->SetSizeCoefficient(0.75f)
						->AddTab("Graph", ETabState::OpenedTab)
					)
				)
			);
	
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bHideSelectionTip = true;

	DetailsViewWidget = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsViewWidget->SetObject(EditingAsset);
	
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, FName("RarityPoolEditor"), StandaloneDefaultLayout, true, true, Asset, false);
	
	RegenerateMenusAndToolbars();
}

void FAssetEditor_RarityPool::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditingAsset);
}

void FAssetEditor_RarityPool::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	TSharedRef<FWorkspaceItem> NewWorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_GraphEditor", "Graph Editor"));
	
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	
	InTabManager->RegisterTabSpawner("Graph", FOnSpawnTab::CreateSP(this, &FAssetEditor_RarityPool::SpawnTab_GraphPanelv2))
	.SetDisplayName(LOCTEXT("GraphCanvasTab", "Graph"))
	.SetGroup(NewWorkspaceMenuCategory)
	.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));
	
	InTabManager->RegisterTabSpawner("RarityPoolDetails", FOnSpawnTab::CreateSP(this, &FAssetEditor_RarityPool::SpawnTab_DetailsPanel))
	.SetDisplayName(LOCTEXT("DetailsTab", "Details"))
	.SetGroup(NewWorkspaceMenuCategory)
	.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));;
}

void FAssetEditor_RarityPool::ExtendToolbar()
{
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
    
	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::Position::After,
		ToolkitCommands,
		FToolBarExtensionDelegate::CreateSP(this, &FAssetEditor_RarityPool::FillToolbar)
	);

	AddToolbarExtender(ToolbarExtender);
}

void FAssetEditor_RarityPool::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.BeginSection("CompileSection");
	{
		ToolbarBuilder.AddToolBarButton(
			FUIAction(
				FExecuteAction::CreateLambda([this]()
				{
					CompileGraph();
				})
			),
			NAME_None,
			NSLOCTEXT("RarityPool", "CompileLabel", "Compile"),
			NSLOCTEXT("RarityPool", "CompileTooltip", "Compile rarity pool into proper weight list"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Recompile")
		);
	}
	
	ToolbarBuilder.EndSection();
}

void FAssetEditor_RarityPool::CreateEdGraph()
{
	
}

TSharedRef<SDockTab> FAssetEditor_RarityPool::SpawnTab_GraphPanelv2(const FSpawnTabArgs& Args)
{
	if (!EditingAsset->EditorGraph)
	{
		EditingAsset->EditorGraph = FBlueprintEditorUtils::CreateNewGraph(
			EditingAsset, 
			NAME_None, 
			UEdGraph::StaticClass(), 
			URarityPollGraphSchema::StaticClass()
		);
		
		EditingAsset->EditorGraph->SetFlags(RF_Transactional | RF_Public);
        EditingAsset->EditorGraph->GetSchema()->CreateDefaultNodesForGraph(*EditingAsset->EditorGraph);
	}
	
	SGraphEditor::FGraphEditorEvents InEvents;
    
	InEvents.OnSelectionChanged.BindSP(this, &FAssetEditor_RarityPool::OnSelectedNodesChanged);
	InEvents.OnTextCommitted.BindSP(this, &FAssetEditor_RarityPool::OnTextChangeCommited);
	
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("CornerText", "Rarity Pool");

	return SNew(SDockTab)
		.Label(FText::FromString(TEXT("Graph")))
		.TabRole(ETabRole::MajorTab)
		[
			SAssignNew(GraphEditor, SGraphEditor)
				   .IsEditable(true)
				   .Appearance(AppearanceInfo)
				   .AdditionalCommands(ToolkitCommands)
			       .GraphEvents(InEvents)
				   .GraphToEdit(EditingAsset->EditorGraph)
		];
}

TSharedRef<SDockTab> FAssetEditor_RarityPool::SpawnTab_DetailsPanel(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
	.Label(LOCTEXT("RarityPoolDetailsTitle", "Details"))
	[
		DetailsViewWidget.ToSharedRef()
	];
}

void FAssetEditor_RarityPool::AddComment()
{
	UEdGraphNode_Comment* const CommentTemplate = NewObject<UEdGraphNode_Comment>();

    FVector2D SpawnLocation = FVector2D::ZeroVector;
    FSlateRect Bounds;

    TSharedPtr<SGraphEditor> GraphEditorPtr = SGraphEditor::FindGraphEditorForGraph(EditingAsset->EditorGraph);
    if (GraphEditorPtr.IsValid())
    {
    	// If they have a selection, build a bounding box around the selection
    	if (GraphEditorPtr->GetBoundsForSelectedNodes(Bounds, 50.0f))
    	{
    		CommentTemplate->SetBounds(Bounds);
    		SpawnLocation.X = CommentTemplate->NodePosX;
    		SpawnLocation.Y = CommentTemplate->NodePosY;
    	}
    	else
    	{
    		// Otherwise initialize a default comment at the user's cursor location.
    		SpawnLocation = GraphEditorPtr->GetPasteLocation();
    	}
    }
	
	FEdGraphSchemaAction_NewNode::SpawnNodeFromTemplate<UEdGraphNode_Comment>(EditingAsset->EditorGraph, CommentTemplate, SpawnLocation, true);
}

void FAssetEditor_RarityPool::OnTextChangeCommited(const FText& Text, ETextCommit::Type Type, UEdGraphNode* Node)
{
	if (UEdGraphNode_Comment* Comment = Cast<UEdGraphNode_Comment>(Node))
	{
		Comment->NodeComment = Text.ToString();
		Comment->Modify();
	}
}

void FAssetEditor_RarityPool::OnEnableSelectedNodes()
{
	if (!EditingAsset->EditorGraph) return;
	
	const FText TransactionTitle = NSLOCTEXT("RarityPoolGraph", "EnableSelectedNodes", "Enable Graph Nodes");
	const FScopedTransaction Transaction(TransactionTitle);
	
	FGraphPanelSelectionSet SelectedNodes = GraphEditor->GetSelectedNodes();
	
	for (UObject* NodeObj : SelectedNodes)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(NodeObj))
		{
			Node->GetGraph()->Modify();
			Node->SetEnabledState(ENodeEnabledState::Enabled);
			Node->GetGraph()->NotifyGraphChanged();
		}
	}
}

void FAssetEditor_RarityPool::OnDisableSelectedNodes()
{
	if (!EditingAsset->EditorGraph) return;
	
	const FText TransactionTitle = NSLOCTEXT("RarityPoolGraph", "DisableSelectedNodes", "Disable Graph Nodes");
	const FScopedTransaction Transaction(TransactionTitle);
	
	FGraphPanelSelectionSet SelectedNodes = GraphEditor->GetSelectedNodes();
	
	for (UObject* NodeObj : SelectedNodes)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(NodeObj))
		{
			Node->GetGraph()->Modify();
			Node->SetEnabledState(ENodeEnabledState::Disabled);
			Node->GetGraph()->NotifyGraphChanged();
		}
	}
}

void FAssetEditor_RarityPool::DeleteSelectedNodes()
{
	if (!EditingAsset->EditorGraph) return;
	
	const FText TransactionTitle = NSLOCTEXT("RarityPoolGraph", "DeleteSelectedNodes", "Delete Graph Nodes");
	const FScopedTransaction Transaction(TransactionTitle);
	
	FGraphPanelSelectionSet SelectedNodes = GraphEditor->GetSelectedNodes();
	
	for (UObject* NodeObj : SelectedNodes)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(NodeObj))
		{
			if (Node->CanUserDeleteNode())
			{
				Node->GetGraph()->Modify();
				Node->GetGraph()->RemoveNode(Node);
			}
		}
	}
}

void FAssetEditor_RarityPool::OnSelectedNodesChanged(const FGraphPanelSelectionSet& SelectionSet)
{
	for (UObject* Node : SelectionSet)
	{
		// Put or statements for the rest of the types that are wanted to allow showing details.
		// For now comments for color & other modifications.
		if (Node->IsA(UEdGraphNode_Comment::StaticClass()))
		{
			DetailsViewWidget->SetObject(Node);
			return;
		}
		
		// Specificly added for modifying gameplay tag nodes
		if (UEdGraphNode_RarityPollItem* RarityNode = Cast<UEdGraphNode_RarityPollItem>(Node))
		{
			if (RarityNode->bCanSeeDetails)
			{
				DetailsViewWidget->SetObject(RarityNode);
				return;
			}
		}
	}
	
	DetailsViewWidget->SetObject(EditingAsset);
}

bool FAssetEditor_RarityPool::CanDeleteNodes()
{
	return true;
}

void FAssetEditor_RarityPool::CompileGraph()
{
	if (EditingAsset->EditorGraph)
	{
		EditingAsset->Items.Empty();
						
		TArray<UEdGraphNode_RarityPollItem*> Nodes; 
		EditingAsset->EditorGraph->GetNodesOfClass(Nodes);
		if (Nodes.Num() == 0) return;
		Nodes = Nodes.FilterByPredicate([](UEdGraphNode_RarityPollItem* Node){ return Node != nullptr ? Node->GetDesiredEnabledState() == ENodeEnabledState::Enabled : false; });
		
		for (UEdGraphNode_RarityPollItem* Node : Nodes)
		{
			FItemDefiner Item;
			Item.Item = Node->Item;
			Item.Rarity = Node->Rarity;
			
			EditingAsset->Items.Add(Item);
		}
		
		EditingAsset->EditorGraph->NotifyGraphChanged();
		SaveAsset_Execute();
	}
}