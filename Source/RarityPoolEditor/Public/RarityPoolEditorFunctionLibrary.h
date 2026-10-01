// Paridyz

#pragma once

#include "CoreMinimal.h"
#include "Graph/EdGraphNode_RarityPollItem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RarityPoolEditorFunctionLibrary.generated.h"

UCLASS()
class RARITYPOOLEDITOR_API URarityPoolEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Rarity Pool")
	static ENodeEnabledState GetNodeEnabledState(UEdGraphNode_RarityPollItem* Node)
	{
		return Node != nullptr ? Node->GetDesiredEnabledState() : ENodeEnabledState::Enabled;
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Rarity Pool")
	static float GetNodeRarity(UEdGraphNode_RarityPollItem* Node)
	{
		return Node != nullptr ? Node->Rarity : 0.0f;
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Rarity Pool")
	static float GetNodeRarityPercentage(UEdGraphNode_RarityPollItem* Node)
	{
		return Node != nullptr ? Node->PercentChance : 0.0f;
	}
	
	UFUNCTION(BlueprintCallable, Category="Rarity Pool")
	static void SetNodeRarity(UEdGraphNode_RarityPollItem* Node, float NewRarity)
	{
		if (Node)
		{
			Node->UpdateAndRecalculateRarity(NewRarity, true);
		}
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Rarity Pool")
	static FInstancedStruct GetItem(UEdGraphNode_RarityPollItem* Node)
	{
		return Node != nullptr ? Node->Item : FInstancedStruct();
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Rarity Pool")
	static FSlateBrush GetClassThumbnail(UClass* Class);
};
