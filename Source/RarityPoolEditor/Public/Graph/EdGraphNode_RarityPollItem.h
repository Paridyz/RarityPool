// Paridyz

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "EdGraph/EdGraphNode.h"
#include "StructUtils/InstancedStruct.h"
#include "EdGraphNode_RarityPollItem.generated.h"

UCLASS(BlueprintType)
class RARITYPOOLEDITOR_API UEdGraphNode_RarityPollItem : public UEdGraphNode
{
	GENERATED_BODY()
	
public:
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
	{
		return FText::FromString(FString::Printf(TEXT("")));
	}
	
	virtual FLinearColor GetNodeTitleColor() const override { return FColor(232, 52, 235); }

	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	
	virtual void AllocateDefaultPins() override 
	{
		CreatePin(EGPD_Output, TEXT("ItemPin"), FName(TEXT("Out")));
	}
	
	void CreateInputSideAddButton(TSharedPtr<SVerticalBox> InputBox);
	
	void UpdateAndRecalculateRarity(float NewRarity, bool bIsRecursive);

public:
	UPROPERTY(meta = (ShowOnNode))
	float Rarity = 1.0f;
	
	UPROPERTY(meta = (ShowOnNode))
	float PercentChance = 100.0f;
	
	UPROPERTY(meta = (ShowOnNode))
	bool bCanSeeDetails = false;
	
	UPROPERTY(EditAnywhere)
	FInstancedStruct Item;
};


class SRarityPollItemNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SRarityPollItemNode){}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, UEdGraphNode_RarityPollItem* InNode)
	{
		GraphNode = InNode;
		UpdateGraphNode();
	}
	
	UUserWidget* NodeWidget;
	virtual void UpdateGraphNode() override;
};

UCLASS()
class RARITYPOOLEDITOR_API UEdGraphNode_RarityMasterOutput : public UEdGraphNode
{
	GENERATED_BODY()
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString(TEXT("Output")); }
	virtual FLinearColor GetNodeTitleColor() const override { return FColor(102, 73, 204); }
	virtual bool CanUserDeleteNode() const override { return false; }

	virtual void AllocateDefaultPins() override 
	{
	}
};