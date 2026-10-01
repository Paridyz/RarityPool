// Paridyz


#include "Graph/EdGraphNode_RarityPollItem.h"
#include "Graph/UI/RarityPoolNodeWidget.h"

#include "Widgets/Input/SNumericEntryBox.h"


TSharedPtr<SGraphNode> UEdGraphNode_RarityPollItem::CreateVisualWidget()
{
	return SNew(SRarityPollItemNode, this);
}

void UEdGraphNode_RarityPollItem::CreateInputSideAddButton(TSharedPtr<SVerticalBox> InputBox)
{
	if (UEdGraphPin* RarityPin = FindPin(TEXT("Rarity")))
	{
		InputBox->AddSlot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				// Expose the raw data field in Slate next to or inside the pin slot!
				SNew(SNumericEntryBox<float>)
				.Value_Lambda([this]() { return Rarity; })
				.OnValueChanged_Lambda([this](float NewVal) { Rarity = NewVal; })
			]
		];
	}
}

void UEdGraphNode_RarityPollItem::UpdateAndRecalculateRarity(float NewRarity, /* Go through & update all other nodes? */ bool bIsRecursive)
{
	// Update & calculate rarity % :D
	
	Rarity = NewRarity;
	
	TArray<UEdGraphNode_RarityPollItem*> Nodes; 
	GetGraph()->GetNodesOfClass(Nodes);
	if (Nodes.Num() == 0) return;
	Nodes = Nodes.FilterByPredicate([](UEdGraphNode_RarityPollItem* Node){ return Node != nullptr ? Node->GetDesiredEnabledState() == ENodeEnabledState::Enabled : false; });
		
	float TotalWeight = 0.0f;
		
	for (UEdGraphNode_RarityPollItem* Node : Nodes)
	{
		TotalWeight += Node->Rarity;
		
		if (bIsRecursive)
		{
			Node->UpdateAndRecalculateRarity(Node->Rarity, false);
		}
	}
		
	PercentChance = (Rarity / TotalWeight) * 100;
	
	GetGraph()->NotifyGraphChanged();
}

void SRarityPollItemNode::UpdateGraphNode()
{
	// SGraphNode::UpdateGraphNode();
	
	if (UEdGraphNode_RarityPollItem* RarityPollItemNode = Cast<UEdGraphNode_RarityPollItem>(GraphNode))
		RarityPollItemNode->UpdateAndRecalculateRarity(RarityPollItemNode->Rarity, false);
	
	if (!NodeWidget)
	{
		TSharedPtr<SWidget> EmbeddedWidget = SNullWidget::NullWidget;
		UClass* WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/RarityPool/UI/EUW_Node.EUW_Node_C"));
		URarityPoolNodeWidget* CreatedWidget = NewObject<URarityPoolNodeWidget>(GetTransientPackage(), WidgetClass);
		CreatedWidget->Node = Cast<UEdGraphNode_RarityPollItem>(GraphNode);
		CreatedWidget->Initialize();	
		NodeWidget = CreatedWidget;
	}
	
	URarityPoolNodeWidget* Widget = Cast<URarityPoolNodeWidget>(NodeWidget);
	Widget->UpdateWidget();
	
	GetOrAddSlot(ENodeZone::Center)
	[
		SNew(SBox)
		.HAlign(HAlign_Fill)
		.HAlign(HAlign_Fill)
		.Padding(0.f)
		.WidthOverride(250)
		.HeightOverride(100)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				NodeWidget->TakeWidget()
			]
		]
	];
}