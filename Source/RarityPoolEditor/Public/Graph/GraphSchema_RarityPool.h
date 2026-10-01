// Paridyz

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_RarityPool.h"
#include "EdGraphNode_RarityPollItem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/Object.h"
#include "GraphSchema_RarityPool.generated.h"

class FNewNodeAction_RarityPoll : public FEdGraphSchemaAction
{
public:
	FNewNodeAction_RarityPoll() {}
	FNewNodeAction_RarityPoll(FText InMenuDesc, FText InToolTip, FText InCategory, FInstancedStruct InItem, /* If we can see the nodes inner details */ bool InCanSeeDetails = false) 
	: FEdGraphSchemaAction(InCategory, InMenuDesc, InToolTip, 0) {
		Item = InItem;
		bCanSeeDetails = InCanSeeDetails;
	}
	
	UPROPERTY()
	FInstancedStruct Item;
	
	UPROPERTY()
	bool bCanSeeDetails = false;

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override
	{
		const FScopedTransaction Transaction(FText::FromString(TEXT("Rarity Pool Graph: Add Item Node")));
		ParentGraph->Modify();

		UEdGraphNode_RarityPollItem* NewNode = NewObject<UEdGraphNode_RarityPollItem>(ParentGraph);
    
		NewNode->SetFlags(RF_Transactional);
		NewNode->CreateNewGuid();
    
		NewNode->NodePosX = Location.X;
		NewNode->NodePosY = Location.Y;
		NewNode->Item = Item;
		NewNode->Rarity = 1.0f;
		NewNode->bCanSeeDetails = bCanSeeDetails; /* Controls if it shows up in the details view panel */
    
		NewNode->AllocateDefaultPins();
    
		ParentGraph->AddNode(NewNode, true, bSelectNewNode);
		
		// Update all previous nodes
		TArray<UEdGraphNode_RarityPollItem*> Nodes; 
		ParentGraph->GetNodesOfClass(Nodes);
		
		for (UEdGraphNode_RarityPollItem* Node : Nodes)
			Node->UpdateAndRecalculateRarity(Node->Rarity, false);
    
		return NewNode;
	}
};

UCLASS()
class RARITYPOOLEDITOR_API URarityPollGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("No connections!"));
	}
	
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual bool ShouldHidePinDefaultValue(UEdGraphPin* Pin) const override;

	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override
	{
		Super::GetGraphContextActions(ContextMenuBuilder);
		
		FText Category_Core = FText::FromString(TEXT("Core"));
		
		TSharedPtr<FNewNodeAction_RarityPoll> TagAction = MakeShared<FNewNodeAction_RarityPoll>(
			FText::FromString("New Gameplay Tag Item"),
			FText::FromString("Makes new random item that returns a gameplay tag."),
			Category_Core,
			FInstancedStruct::Make(FTagRarityItem()),
			true /* Allow viewing & editing of the tag */
		);
							
		ContextMenuBuilder.AddAction(TagAction);
		
		FText Category = FText::FromString(TEXT("Items"));
		
		UDataAsset_RarityPool* Poll = Cast<UDataAsset_RarityPool>(ContextMenuBuilder.CurrentGraph->GetOuter());
		
		if (!Poll) return;
		if (Poll->AllowedClasses.IsEmpty()) return;
		
		IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();

		FARFilter Filter;
		Filter.bRecursiveClasses = true;
    
		for (UClass* AllowedClass : Poll->AllowedClasses)
		{
			if (AllowedClass)
			{
				Filter.ClassPaths.Add(AllowedClass->GetClassPathName());	
			}
		}
		Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName()); 

		TArray<FAssetData> FoundAssets;
		AssetRegistry.GetAssets(Filter, FoundAssets);
		
		TSet<FTopLevelAssetPath> DerivedClassPaths;
		{
			TArray<FTopLevelAssetPath> BaseClassPaths;
			for (UClass* AllowedClass : Poll->AllowedClasses)
			{
				if (AllowedClass)
				{
					BaseClassPaths.Add(AllowedClass->GetClassPathName());
					DerivedClassPaths.Add(AllowedClass->GetClassPathName());
				}
			}
        
			TSet<FTopLevelAssetPath> ExcludedClassPaths;
			AssetRegistry.GetDerivedClassNames(BaseClassPaths, ExcludedClassPaths, DerivedClassPaths);
		}
		
		/* At this point, I dont even know what is happening and I just want this stupid filter to be done, 2 filters 1 blueprint, 1 normal. cool. - Paridyz */
		
		// Filter through blueprint assets specifically
		for (FAssetData Asset : FoundAssets)
		{
			FTopLevelAssetPath AssetClassPath = Asset.AssetClassPath;
			if(AssetClassPath == UBlueprint::StaticClass()->GetClassPathName())
			{
				// Replaced from our actual code in Fortress, In Fortress we used a helper function - Paridyz
				FText DisplayName = FText::FromString(Asset.AssetName.ToString());
				FText Tooltip = FText::FromString(Asset.AssetName.ToString());
				FSoftObjectPath ObjectPath(FString::Printf(TEXT("%s.%s_C"), *Asset.PackageName.ToString(), *Asset.AssetName.ToString()));
				
				FAssetTagValueRef ParentClassRef = Asset.TagsAndValues.FindTag(FName("ParentClass"));
				if (ParentClassRef.IsSet())
				{
					FString CleanPath = FPackageName::ExportTextPathToObjectPath(ParentClassRef.GetValue());
					FTopLevelAssetPath ParentClassPath(CleanPath);

					if (DerivedClassPaths.Contains(ParentClassPath))
					{
						FSoftRarityItem SoftRarityItem;
						SoftRarityItem.Item = ObjectPath;
						
						TSharedPtr<FNewNodeAction_RarityPoll> NewAction = MakeShared<FNewNodeAction_RarityPoll>(
							DisplayName,
							Tooltip,
							Category,
							FInstancedStruct::Make(SoftRarityItem)
						);
							
						ContextMenuBuilder.AddAction(NewAction);
					}
				}
			}
		}
		
		FoundAssets.Empty();
		Filter = FARFilter();
		Filter.bRecursiveClasses = true;
		for (UClass* AllowedClass : Poll->AllowedClasses)
		{
			if (AllowedClass)
			{
				Filter.ClassPaths.Add(AllowedClass->GetClassPathName());
			}
		}
		
		AssetRegistry.GetAssets(Filter, FoundAssets);
		
		for (FAssetData Asset : FoundAssets)
		{
			FTopLevelAssetPath AssetClassPath = Asset.AssetClassPath;
			bool bIsBlueprint = (AssetClassPath == UBlueprint::StaticClass()->GetClassPathName());
			
			FText DisplayName = FText::FromString(Asset.AssetName.ToString());
			FText Tooltip = FText::FromString(Asset.AssetName.ToString());
			FSoftObjectPath ObjectPath(FString::Printf(TEXT("%s.%s"), *Asset.PackageName.ToString(), *Asset.AssetName.ToString()));
			
			FSoftRarityItem SoftRarityItem;
			SoftRarityItem.Item = ObjectPath;
			
			TSharedPtr<FNewNodeAction_RarityPoll> NewAction = MakeShared<FNewNodeAction_RarityPoll>(
				DisplayName,
				Tooltip,
				Category,
				FInstancedStruct::Make(SoftRarityItem)
			);
				
			ContextMenuBuilder.AddAction(NewAction);
		}
	}
};